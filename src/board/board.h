//
// Created by edson-moraes on 1/12/21.
//
#pragma once
#ifndef LIFE_C_BOARD_H
#define LIFE_C_BOARD_H

#include <stdint.h>
#include <stdbool.h>


#define ALIVE 255

#define UNDERPOP_THRESHOLD 2
#define OVERPOP_THRESHOLD 3
#define REPRODUCTION_VALUE 3


typedef struct life_board {
    int *c_board;
    int *n_board;
    int rows;
    int columns;
    int cell_size;
} life_board;

life_board *init_board(int rows, int columns, int cell_size);

int set_random_pattern(life_board *board);

int get(life_board *board, int x, int y);

void set(life_board *board, int x, int y, int value, bool curr);

void advance_generation(life_board *board);

void free_board(life_board *b);


#endif //LIFE_C_BOARD_H
