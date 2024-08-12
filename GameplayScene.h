#pragma once

// #include "DungeonManager.h"
#include "DungeonFloor.h"
#include "Scene.h"

class GameplayScene : public Scene {
public:
  GameplayScene();
  ~GameplayScene();

  void update();
  bool init();
  void handle_input();
  void handle_player_input();
  void handle_camera_input();

  void draw_debug_panel();
  void draw_hud();
  void draw();

  void close();

  void cleanup();
  void gameover();
  inline void handle_draw_debug_panel();
  inline void handle_popup_manager();

  void update_player_movement();
  void update_enemy_movement();
  void handle_player_collision();
  void handle_dungeon_move(entity_id id, Vector2 direction);

  entity_id spawn_player(float x, float y);

  void set_scale(const float f);
  entity_id spawn_tile_stone(float i, float j, float x, float y);
  entity_id spawn_tile_void(float x, float y);

private:
  bool show_test_popup = true;

  entity_id player_id = -1;

  unsigned int turn_count = 0;

  DungeonFloor dungeon_floor;
};
