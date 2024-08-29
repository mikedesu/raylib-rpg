#include "Game.h"

int main(void) {
  Game game;
  const char *title = "@evildojo666 presents - Project.RPG v0.1";
  game.set_window_title(title);
  game.set_camera_default_values();
  game.set_debug_panel(true);
  game.init();
  game.run();
  return 0;
}
