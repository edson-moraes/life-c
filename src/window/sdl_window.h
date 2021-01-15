//
// Created by edson-moraes on 1/11/21.
//
#pragma once
#ifndef LIFE_C_SDL_WINDOW_H
#define LIFE_C_SDL_WINDOW_H

#include <stdint.h>
#include <SDL2/SDL.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <X11/Xproto.h>

#include "../board/board.h"

typedef struct sdl_window {
    SDL_Renderer *renderer;
    SDL_Window *window;
    Window x_overlay;
    Display *x_display;
    SDL_Texture *cell_texture;
} sdl_window;

sdl_window init_window(int *screen_width, int *screen_height);

void draw_board(life_board *board, sdl_window window);

void destroy_window(sdl_window window);

bool wait();

#endif //LIFE_C_SDL_WINDOW_H
