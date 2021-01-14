//
// Created by edson-moraes on 1/12/21.
//

#include <stdlib.h>
#include <stdio.h>
#include "board.h"

life_board *init_board(int rows, int columns, int cell_size) {
    life_board *result = malloc(sizeof(life_board));

    result->rows = rows;
    result->columns = columns;
    result->cell_size = cell_size;

    result->c_board = calloc(rows * columns, sizeof(int));
    result->n_board = calloc(rows * columns, sizeof(int));

    return result;
}


int get(life_board *board, int x, int y) {
    return board->c_board[x * board->columns + y];
}

void set(life_board *board, int x, int y, int value, bool curr) {
    if (curr) {
        board->c_board[x * board->columns + y] = value;
    } else {
        board->n_board[x * board->columns + y] = value;
    }
}


int count_neighbours(life_board *board, int x, int y) {
    int alive_neighbour_count = 0;

    int new_x, new_y;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {

            new_x = x + i;
            new_y = y + j;

            if (new_x < 0 || new_y < 0) continue;
            if (new_x > board->rows - 1 || new_y > board->columns - 1) continue;
            if (new_x == x && new_y == y) continue;
            if (get(board, new_x, new_y) == ALIVE) alive_neighbour_count++;

        }
    }
    return alive_neighbour_count;
}

bool underpopulation_rule(life_board *board, int x, int y) {
    if (get(board, x, y) == ALIVE) {
        if (count_neighbours(board, x, y) < UNDERPOP_THRESHOLD) {
            set(board, x, y, ALIVE / 2, false);
            return true;
        }
    }
    return false;
}

bool overpopulation_rule(life_board *board, int x, int y) {

    if (get(board, x, y) == ALIVE) {
        if (count_neighbours(board, x, y) > OVERPOP_THRESHOLD) {
            set(board, x, y, ALIVE / 2, false);
            return true;
        }
    }
    return false;
}

bool reproduction_rule(life_board *board, int x, int y) {
    if (get(board, x, y) != ALIVE) {
        if (count_neighbours(board, x, y) == REPRODUCTION_VALUE) {
            set(board, x, y, ALIVE, false);
            return true;
        }
    }
    return false;
}

bool survival_rule(life_board *board, int x, int y) {
    if (get(board, x, y) == ALIVE) {
        int live_neighbours = count_neighbours(board, x, y);
        if (live_neighbours >= UNDERPOP_THRESHOLD && live_neighbours <= REPRODUCTION_VALUE) {
            set(board, x, y, ALIVE, false);
            return true;
        }
    }
    return false;
}

void clear(life_board *board) {
    for (int i = 0; i < board->rows * board->columns; i++) {
        board->n_board[i] = 0;
    }
}

void advance_generation(life_board *board) {

    for (int i = 0; i < board->rows; i++) {
        for (int j = 0; j < board->columns; j++) {
            if (underpopulation_rule(board, i, j)) continue;
            if (overpopulation_rule(board, i, j)) continue;
            if (reproduction_rule(board, i, j)) continue;
            if (survival_rule(board, i, j)) continue;
            set(board, i, j, get(board, i, j) / 2, false);
        }
    }
    int *tmp = board->c_board;
    board->c_board = board->n_board;
    board->n_board = tmp;
    clear(board);
}

int set_random_pattern(life_board *board) {
    FILE *random_data = fopen("/dev/urandom", "r");
    if (random_data == NULL) {
        return -1;
    }
    uint64_t r;
    for (int i = 0; i < board->rows; i++) {
        for (int j = 0; j < board->columns; j++) {
            fread(&r, sizeof(uint64_t), 1, random_data);
            set(board, i, j, (uint64_t) ((r % 2L) * ALIVE), true);
        }
    }
    fclose(random_data);
    return 0;
}

void free_board(life_board *b) {
    free(b->c_board);
    free(b->n_board);
    free(b);
}
