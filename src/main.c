#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#include "window/sdl_window.h"
#include "board/board.h"



//Screen dimension constants
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#define CELL_SIZE 10
#define BOARD_WIDTH SCREEN_WIDTH/CELL_SIZE
#define BOARD_HEIGHT SCREEN_HEIGHT/CELL_SIZE


int main() {

    sdl_window *window = init_window(SCREEN_WIDTH, SCREEN_HEIGHT);
    life_board *board = init_board(BOARD_HEIGHT, BOARD_WIDTH);
    set_random_pattern(board);

    while (wait()) {
        draw_board(board, window);
        advance_generation(board);
        SDL_Delay(166);
    }

    destroy_window(window);
    return 0;
}
