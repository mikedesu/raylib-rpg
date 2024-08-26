#pragma once

#include "DungeonEvent.h"
#include "DungeonFloor.h"
#include "Scene.h"

#include <vector>

class GameplayScene : public Scene {
public:
  GameplayScene();
  ~GameplayScene();

  bool init();
  bool handle_dungeon_move_pos(const entity_id id, const Vector2 c_pos,
                               const Vector2 t_pos);

  void update();
  void handle_input();
  void handle_control_mode_switch();
  void close();
  void cleanup();
  void gameover();
  void init_dungeon_floor();
  void set_scale(const float f);
  void increase_lighting_at(const Vector2 loc, const int light_level);
  void decrease_lighting_at(const Vector2 loc, const int light_level);

  inline void handle_player_input();
  inline void handle_player_move_direction();
  inline void handle_player_mouse_click();
  inline void handle_camera_input();
  inline void handle_camera_input_move();
  inline void handle_camera_input_zoom();

  inline void draw_debug_panel();
  inline void draw_hud();
  inline void draw_message_log();
  inline void draw_controls();
  inline void draw();
  inline void draw_tile(const string tile_key, const int i, const int j);
  inline void handle_popup_manager();
  inline void handle_tile_click(const Rectangle dest, const int i, const int j);

  Vector2 handle_dungeon_move_dir(const entity_id id, const Vector2 direction);

  const string tile_key_for_type(const tile_type t) const;

  const Vector2 get_start_location() const;

  const entity_id spawn_player(const Vector2 pos);
  const entity_id spawn_column(const Vector2 pos);
  const entity_id spawn_torch(const Vector2 pos);

  const string get_dungeon_event_str(const DungeonEvent &dungeon_event);

private:
  bool show_test_popup = true;

  entity_id player_id = -1;

  unsigned int turn_count = 0;

  float prev_tile_click_zoom_level = 1.0f;
  float tile_click_zoom_level = 1.0f;

  DungeonFloor dungeon_floor;

  vector<DungeonEvent> dungeon_events;

  bool player_did_move = false;
  bool player_attempted_move = false;
  bool tile_is_selected = false;
  bool display_message_log = true;

  Vector2 last_mouse_click_pos;
  Vector2 last_tile_click_pos;
};
