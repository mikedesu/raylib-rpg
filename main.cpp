#include "Game.h"

int main(void) {
    Game game;
    const char* title = "@evildojo666 presents - Project.RPG v0.1";
    game.set_window_title(title);
    game.set_camera_default_values();
    game.set_debug_panel(true);
    game.init();
    game.run();

    //game_set_window_title(game, title);
    //game_set_camera_default_values(game);
    //game_set_debug_panel(game, true);
    //game_init(game);
    //game_run(game);

    return 0;
}
