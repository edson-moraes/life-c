//
// Created by edson-moraes on 1/11/21.
//

#include "sdl_window.h"

void
get_x_desktop_window(Window *return_desktop_window, Display **return_x_display, int *desktop_width,
                     int *desktop_height) {

    Window desktop_window, root_return, parent;
    Window *children;
    unsigned int nchildren;

    Display *x_display = XOpenDisplay(0);
    Window x_root_window = DefaultRootWindow(x_display);
    XQueryTree(x_display, x_root_window, &root_return, &parent, &children, &nchildren);
    for (unsigned int i = 0; i < nchildren; i++) {
        char *window_name;
        XFetchName(x_display, children[i], &window_name);
        if (window_name != NULL) {
            if (strcmp("Desktop", window_name) == 0) {
                desktop_window = children[i];
                free(window_name);
                break;
            }
        }
    }
    free(children);

    XWindowAttributes window_attrs;
    XGetWindowAttributes(x_display, desktop_window, &window_attrs);
    (*desktop_width) = window_attrs.width;
    (*desktop_height) = window_attrs.height;

    XSetWindowAttributes attrs;
    attrs.override_redirect = true;

    XVisualInfo vinfo;
    if (!XMatchVisualInfo(x_display, DefaultScreen(x_display), 32, TrueColor, &vinfo)) {
        printf("No visual found supporting 32 bit color, terminating\n");
        exit(EXIT_FAILURE);
    }
    // these next three lines add 32 bit depth, remove if you dont need and change the flags below
    attrs.colormap = XCreateColormap(x_display, desktop_window, vinfo.visual, AllocNone);
    attrs.background_pixel = 0;
    attrs.border_pixel = 0;

    Window overlay = XCreateWindow(
            x_display, desktop_window,
            0, 0, window_attrs.width, window_attrs.height, 0,
            vinfo.depth, InputOutput,
            vinfo.visual,
            CWOverrideRedirect | CWColormap | CWBackPixel | CWBorderPixel, &attrs
    );

    XStoreName(x_display, overlay, "Conway's Game of Life - Overlay");
    XMapWindow(x_display, overlay);
    XLowerWindow(x_display, overlay);
    XFlush(x_display);

    (*return_desktop_window) = overlay;
    (*return_x_display) = x_display;
}

sdl_window init_window(int *screen_width, int *screen_height) {
    SDL_Renderer *renderer;
    SDL_Window *window;
    Window x_desktop_window = 0;
    Display *x_display = NULL;

    SDL_Init(SDL_INIT_VIDEO);
    get_x_desktop_window(&x_desktop_window, &x_display, screen_width, screen_height);

    window = SDL_CreateWindowFrom((void *) x_desktop_window);
    renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_Surface *image = SDL_LoadBMP("spotlight-big-alpha4.bmp");
    SDL_Texture *cell_texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);

    sdl_window result;
    result.window = window;
    result.renderer = renderer;
    result.x_overlay = x_desktop_window;
    result.x_display = x_display;
    result.cell_texture = cell_texture;

    return result;
}

void draw_board(life_board *board, sdl_window window) {
    SDL_SetRenderDrawColor(window.renderer, 0, 0, 0, 0);
    SDL_RenderClear(window.renderer);

    int cs = board->cell_size;
    for (int i = 0; i < board->rows; i++) {
        for (int j = 0; j < board->columns; j++) {
            SDL_Rect fillRect = {j * cs, i * cs, cs, cs};
            SDL_SetTextureAlphaMod(window.cell_texture, get(board, i, j) / 3);
            SDL_RenderCopy(window.renderer, window.cell_texture, NULL, &fillRect);
        }
    }
    SDL_RenderPresent(window.renderer);
    XLowerWindow(window.x_display, window.x_overlay);
}

void destroy_window(sdl_window window) {
    SDL_DestroyWindow(window.window);
    SDL_DestroyRenderer(window.renderer);
    SDL_DestroyTexture(window.cell_texture);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();

    XUnmapWindow(window.x_display, window.x_overlay);
    XCloseDisplay(window.x_display);
}

