#include "Game.h"

int main(void) {
    Game game;
    const char* title = "@evildojo666 presents - Project.RPG v0.1";
    Game_create(game);
    Game_set_window_title(game, title);
    Game_set_camera_default_values(game);
    Game_set_debug_panel(game, true);
    Game_init(game);
    int success = Game_run(game);
    return success;
}
