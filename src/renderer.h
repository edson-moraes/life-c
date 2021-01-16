//
// Created by edson-moraes on 1/11/21.
//
#pragma once
#ifndef LIFE_C_RENDERER_H
#define LIFE_C_RENDERER_H

#include <stdint.h>
#include <SDL2/SDL.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <X11/Xproto.h>

#include "game.h"

typedef struct renderer {
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Texture *cell_texture;

    Window x_overlay;
    Display *x_display;

    int desktop_width, desktop_height;

} renderer_context;

renderer_context init_renderer_context();

void draw_board(life_board *board, renderer_context window);

void destroy_renderer_context(renderer_context window);

#endif //LIFE_C_RENDERER_H
