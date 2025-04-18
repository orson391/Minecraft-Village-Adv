#include "her/game.h"

int main(int argc, char* argv[]) {
    Game* game = new Game();
    if (!game) {
        std::cerr << "Failed to allocate Game object" << std::endl;
        return 1;
    }

    game->init("MYGAME", 500, 500, 800, 400, false, 75);
    if (!game->running()) {
        std::cerr << "Game initialization failed" << std::endl;
        game->clean();
        delete game;
        return 1;
    }

    while (game->running()) {
        game->handleEvents();
        game->update();
        game->render();
    }

    delete game; // clean() called in ~Game()
    return 0;
}