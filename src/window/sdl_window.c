//
// Created by edson-moraes on 1/11/21.
//

#include "sdl_window.h"

sdl_window *init_window(int screen_width, int screen_height) {
    SDL_Renderer *renderer;
    SDL_Window *window;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(screen_width, screen_height, 0, &window, &renderer);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    sdl_window *result = malloc(sizeof(sdl_window));
    result->window = window;
    result->renderer = renderer;

    return result;
}

void draw_board(life_board *board, sdl_window *window) {
    SDL_Surface *image = SDL_LoadBMP("spotlight-big.bmp");
    SDL_Texture *texture1 = SDL_CreateTextureFromSurface(window->renderer, image);
    SDL_SetTextureBlendMode(texture1, SDL_BLENDMODE_BLEND);

    SDL_RenderClear(window->renderer);

    SDL_RenderPresent(window->renderer);

    for (int i = 0; i < board->rows; i++) {
        for (int j = 0; j < board->columns; j++) {
            int cell_value = get(board, i, j);
            SDL_Rect fillRect = {j * 20, i * 20, 20, 20};
            SDL_SetTextureAlphaMod(texture1, cell_value);
            SDL_RenderCopy(window->renderer, texture1, NULL, &fillRect);
        }
    }

    SDL_RenderPresent(window->renderer);
}

void destroy_window(sdl_window *window) {
    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->window);
    SDL_Quit();
}

bool wait() {
    SDL_Event event;
    if (SDL_PollEvent(&event) && event.type == SDL_QUIT) {
        return false;
    }
    return true;
}

