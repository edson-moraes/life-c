#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>


#define ROWS 100
#define COLUMNS 100

#define DEAD 0
#define ALIVE 255

#define UNDERPOP_THRESHOLD 2
#define OVERPOP_THRESHOLD 3
#define REPRODUCTION_VALUE 3

#define GET(x, y) (x*ROWS + y)

//Screen dimension constants
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

int count_neighbours(const int *board, int x, int y) {
    int neighbour_count = 0;
    int alive_neighbour_count = 0;

    int new_x, new_y;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {

            new_x = x + i;
            new_y = y + j;

            if (new_x < 0 || new_y < 0) continue;
            if (new_x > ROWS - 1 || new_y > COLUMNS - 1) continue;
            if (new_x == x && new_y == y) continue;

            neighbour_count++;
            if (board[GET(new_x, new_y)]) alive_neighbour_count++;

        }
    }
    return alive_neighbour_count;
}

bool underpopulation_rule(int *current_board, int *next_board, int x, int y) {
    if (current_board[GET(x, y)] == ALIVE) {
        if (count_neighbours(current_board, x, y) < UNDERPOP_THRESHOLD) next_board[GET(x, y)] = DEAD;
        return true;
    }
    return false;
}

bool overpopulation_rule(int *current_board, int *next_board, int x, int y) {
    if (current_board[GET(x, y)] == ALIVE) {
        if (count_neighbours(current_board, x, y) > OVERPOP_THRESHOLD) next_board[GET(x, y)] = DEAD;
        return true;
    }
    return false;
}

bool reproduction_rule(int *current_board, int *next_board, int x, int y) {
    if (current_board[GET(x, y)] == DEAD) {
        if (count_neighbours(current_board, x, y) == REPRODUCTION_VALUE) next_board[GET(x, y)] = ALIVE;
        return true;
    }
    return false;
}

void alive_rule(const int *board, int x, int y) {

}

int *next_generation(int *initial_state) {

    int *current_board = initial_state;
    int *next_board = calloc(ROWS * COLUMNS, sizeof(int));

    int it = 0;

    while (it < 1000) {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLUMNS; j++) {
                if (underpopulation_rule(current_board, next_board, i, j)) continue;
                if (overpopulation_rule(current_board, next_board, i, j)) continue;
                if (reproduction_rule(current_board, next_board, i, j)) continue;
            }
        }
        int *tmp = current_board;
        current_board = next_board;
        next_board = tmp;
        memset(next_board, 0, sizeof(int) * ROWS * COLUMNS);
        it++;
    }
}

int main(int argc, char *args[]) {
    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;
    int i;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (i = 0; i < SCREEN_WIDTH; ++i)
        SDL_RenderDrawPoint(renderer, i, i);
    SDL_RenderPresent(renderer);
    while (1) {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}