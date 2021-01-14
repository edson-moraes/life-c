//
// Created by edson-moraes on 1/11/21.
//

#include "sdl_window.h"


void get_x_desktop_window(Window *return_desktop_window, Display **return_x_display) {

    Window desktop_window, root_return, parent;
    Window *children;
    unsigned int nchildren;

    Display *x_display = XOpenDisplay(0);
    Window x_root_window = DefaultRootWindow(x_display);
    XQueryTree(x_display, x_root_window, &root_return, &parent, &children, &nchildren);
    for (unsigned int i = 0; i < nchildren; i++) {
        char *window_name;
        XFetchName(x_display, children[i], &window_name);
        if (window_name != NULL && strcmp("Desktop", window_name) == 0) {
            printf("Window %lu: %s", children[i], window_name);
            desktop_window = children[i];
        }
    }

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

    // Window XCreateWindow(
    //     Display *display, Window parent,
    //     int x, int y, unsigned int width, unsigned int height, unsigned int border_width,
    //     int depth, unsigned int class,
    //     Visual *visual,
    //     unsigned long valuemask, XSetWindowAttributes *attributes
    // );
    Window overlay = XCreateWindow(
            x_display, desktop_window,
            0, 0, 1280, 720, 0,
            vinfo.depth, InputOutput,
            vinfo.visual,
            CWOverrideRedirect | CWColormap | CWBackPixel | CWBorderPixel, &attrs
    );

    XMapWindow(x_display, overlay);

    free(children);
    (*return_desktop_window) = overlay;
    (*return_x_display) = x_display;
}

sdl_window *init_window(int screen_width, int screen_height, bool use_x11) {
    SDL_Renderer *renderer;
    SDL_Window *window;
    Window x_desktop_window = 0;
    Display *x_display = NULL;

    if (use_x11) {
        get_x_desktop_window(&x_desktop_window, &x_display);
        window = SDL_CreateWindowFrom((void *) x_desktop_window);
        renderer = SDL_CreateRenderer(window, -1, 0);

    } else {
        SDL_Init(SDL_INIT_VIDEO);
        SDL_CreateWindowAndRenderer(screen_width, screen_height, 0, &window, &renderer);
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 101, 165, 186, 255);
    SDL_RenderClear(renderer);

    sdl_window *result = malloc(sizeof(sdl_window));
    result->window = window;
    result->renderer = renderer;
    result->x_overlay = x_desktop_window;
    result->x_display = x_display;

    return result;
}

void draw_board(life_board *board, sdl_window *window) {
    //TODO: Alocar apenas uma vez
    SDL_Surface *image = SDL_LoadBMP("spotlight-big-alpha3.bmp");
    SDL_Texture *cell_texture = SDL_CreateTextureFromSurface(window->renderer, image);

    SDL_SetRenderDrawColor(window->renderer, 101, 165, 186, 255);
    SDL_RenderClear(window->renderer);

    int cs = board->cell_size;
    for (int i = 0; i < board->rows; i++) {
        for (int j = 0; j < board->columns; j++) {
            SDL_Rect fillRect = {j * cs, i * cs, cs, cs};
            SDL_SetTextureAlphaMod(cell_texture, get(board, i, j));
            SDL_RenderCopy(window->renderer, cell_texture, NULL, &fillRect);
        }
    }
    SDL_RenderPresent(window->renderer);
    SDL_FreeSurface(image);
}

void destroy_window(sdl_window *window) {

    if (window->x_overlay == 0) {
        SDL_DestroyRenderer(window->renderer);
        SDL_DestroyWindow(window->window);
    } else {
        XUnmapWindow(window->x_display, window->x_overlay);
        XCloseDisplay(window->x_display);
    }

    free(window);
    SDL_Quit();
}

bool wait() {
    SDL_Event event;
    if (SDL_PollEvent(&event) && event.type == SDL_QUIT) {
        return false;
    }
    return true;
}

