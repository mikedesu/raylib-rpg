#include "Game.h"

int main(void) {
  Game game;
  game.set_window_title("@evildojo666 presents - Project.RPG v0.1");
  game.set_camera_default_values();
  game.set_debug_panel(true);
  game.init();
  game.run();
  return 0;
}
