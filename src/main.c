#include "window/sdl_window.h"
#include "board/board.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define CELL_SIZE 10
#define BOARD_WIDTH SCREEN_WIDTH/CELL_SIZE
#define BOARD_HEIGHT SCREEN_HEIGHT/CELL_SIZE

int main() {

    sdl_window *window = init_window(SCREEN_WIDTH, SCREEN_HEIGHT, true);
    life_board *board = init_board(BOARD_HEIGHT, BOARD_WIDTH, CELL_SIZE);
    set_random_pattern(board);

    while (wait()) {
        draw_board(board, window);
        advance_generation(board);
//        SDL_Delay(100);
    }

    destroy_window(window);
    free_board(board);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();
    return 0;
}
