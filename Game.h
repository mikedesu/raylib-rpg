#pragma once

#include "control_mode.h"
#include "entity_id.h"
#include "texture_info.h"

#include "GameoverScene.h"
#include "GameplayScene.h"
#include "PopupManager.h"
#include "Sprite.h"
#include "TitleScene.h"

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using std::function;
using std::make_shared;
using std::shared_ptr;
using std::string;
using std::unordered_map;
using std::vector;

class Game {
public:
  Game();
  ~Game();

  void run();
  void update();
  void draw();
  void handle_input();
  void handle_camera_input();
  void handle_player_input();
  void draw_debug_panel();
  void load_fonts();
  void set_global_scale(float s);
  void set_debug_panel(bool b);
  void set_screen_width(int w);
  void set_screen_height(int h);
  void set_window_title(const char *t);
  void set_camera_default_values();
  void set_has_been_initialized(bool b);
  void close();
  void handle_transition_in();
  void handle_transition_out();
  void cleanup();

  bool get_has_been_initialized();
  bool init();
  string get_window_title();

private:
  unordered_map<string, texture_info> textures;
  unordered_map<entity_id, shared_ptr<Sprite>> sprites;
  unordered_map<entity_id, bool> gravity;
  unordered_map<scene_id, shared_ptr<Scene>> scenes;
  unordered_map<string, scene_id> scene_keys;

  vector<entity_id> entity_ids;

  shared_ptr<Scene> current_scene;
  shared_ptr<PopupManager> popup_manager;

  entity_id player_id;
  scene_id current_scene_id;

  bool has_been_initialized;
  bool debug_panel_on;
  float global_scale;
  unsigned int current_frame;
  control_mode controlmode;
  string window_title;

  Font global_font;
  Camera2D camera2d;
  RenderTexture target;
  Rectangle screen_rect;
};
