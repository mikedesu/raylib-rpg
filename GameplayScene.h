#pragma once

#include "Scene.h"

class GameplayScene : public Scene {
public:
  GameplayScene();
  ~GameplayScene();

  void update();
  bool init();
  void handle_input();

  void draw_debug_panel();
  void draw_hud();
  void draw();

  void close();

  void cleanup();
  void gameover();
  inline void handle_draw_debug_panel();

  void update_player_movement();
  void update_enemy_movement();
  void handle_player_collision();

  entity_id spawn_player(float x, float y);

  void set_scale(const float f);

private:
  bool show_test_popup = true;

  entity_id player_id = -1;
};
