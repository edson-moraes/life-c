#include "renderer.h"
#include "game.h"

#include <time.h>

#define CELL_SIZE 10

int main() {

    time_t begin, end;

    renderer_context rc = init_renderer_context();
    life_board *board = init_game(rc.desktop_height / CELL_SIZE,
                                  rc.desktop_width / CELL_SIZE, CELL_SIZE);
    set_random_pattern(board);

    time(&begin);
    end = begin;
    while (end - begin < 15) {
        draw_board(board, rc);
        advance_generation(board);
        SDL_Delay(100);
        time(&end);
    }

    destroy_renderer_context(rc);
    destroy_game(board);
    return 0;
}
