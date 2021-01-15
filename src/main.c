#include "window/sdl_window.h"
#include "board/board.h"

#include <time.h>

#define CELL_SIZE 10

int main() {

    time_t begin, end;
    int desktop_width, desktop_height;

    sdl_window window = init_window(&desktop_width, &desktop_height);
    life_board *board = init_board(desktop_height / CELL_SIZE, desktop_width / CELL_SIZE, CELL_SIZE);
    set_random_pattern(board);

    time(&begin);
    end = begin;
    while (end - begin < 15) {
        draw_board(board, window);
        advance_generation(board);
        SDL_Delay(100);
        time(&end);
    }

    destroy_window(window);
    free_board(board);
    return 0;
}
