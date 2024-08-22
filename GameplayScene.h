#pragma once

#include "DungeonFloor.h"
#include "Scene.h"
#include <vector>

class GameplayScene : public Scene {
public:
  GameplayScene();
  ~GameplayScene();

  void update();
  bool init();
  void handle_input();
  inline void handle_player_input();
  inline void handle_camera_input();

  inline void draw_debug_panel();
  inline void draw_hud();
  inline void draw_message_log();
  inline void draw_controls();
  inline void draw();
  inline void draw_tile(const string tile_key, const int i, const int j);
  inline void handle_popup_manager();

  const string tile_key_for_type(const tile_type t);

  void close();
  void cleanup();
  void gameover();

  // inline void update_player_movement();
  // inline void update_enemy_movement();
  // inline void handle_player_collision();

  // bool handle_dungeon_move_dir(const entity_id id, const Vector2 direction);
  // bool handle_dungeon_move_pos(const entity_id id, const Vector2 c_pos,
  //                              const Vector2 t_pos);

  Vector2 handle_dungeon_move_dir(const entity_id id, const Vector2 direction);
  bool handle_dungeon_move_pos(const entity_id id, const Vector2 c_pos,
                               const Vector2 t_pos);

  void set_scale(const float f);

  const entity_id spawn_player(const Vector2 pos);
  const entity_id spawn_column(const Vector2 pos);
  // const entity_id spawn_goblin(const Vector2 pos);
  // const entity_id spawn_torch(const Vector2 pos);

  // entity_id spawn_tile_stone(float i, float j, float x, float y);
  // entity_id spawn_tile_void(float x, float y);

private:
  bool show_test_popup = true;

  entity_id player_id = -1;

  unsigned int turn_count = 0;

  DungeonFloor dungeon_floor;

  vector<string> message_log;

  bool player_did_move = false;
  bool player_attempted_move = false;
  bool tile_is_selected = false;
  bool display_message_log = true;

  Vector2 last_mouse_click_pos;
  Vector2 last_tile_click_pos;
};
