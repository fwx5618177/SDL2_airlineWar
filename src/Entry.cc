#include "Entry.hpp"

int Entry() {
    Game game;

    bool success = game.Initialize();

    if (success) {
        game.RunLoop();
    }

    game.Shutdown();

    return 0;
}