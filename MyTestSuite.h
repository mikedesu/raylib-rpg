#include <cxxtest/TestSuite.h>

#include "Game.h"
#include "Sprite.h"
// #include "mPrint.h"

class MyTestSuite : public CxxTest::TestSuite {
public:
  void test_sprite_with_no_texture_filepath() {
    Sprite s("", 1, 0, 0, SPRITETYPE_NONE);
    TS_ASSERT(true);
  }

  void test_game_no_run() {
    Game game;
    game.set_window_title("Game");
    game.set_camera_default_values();
    game.set_debug_panel(true);
    game.init();
    // game.run();
  }

private:
};
