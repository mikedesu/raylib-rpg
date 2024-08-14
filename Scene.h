#pragma once

#include "PopupManager.h"
#include "Sprite.h"
#include "control_mode.h"
#include "entity_id.h"
#include "raylib.h"
#include "texture_info.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using std::make_shared;
using std::shared_ptr;
using std::string;
using std::unordered_map;
using std::vector;

typedef int scene_id;

typedef enum {

  SCENE_TRANSITION_NONE,
  SCENE_TRANSITION_IN,
  SCENE_TRANSITION_OUT,

} scene_transition;

typedef enum {

  SCENE_TYPE_TITLE,
  SCENE_TYPE_GAMEPLAY,
  SCENE_TYPE_GAMEOVER,

} scene_type;

class Scene {
public:
  Scene();
  ~Scene();

  virtual void update();
  virtual bool init();
  virtual void handle_input();
  virtual void draw_debug_panel();
  virtual void cleanup();
  virtual void draw_hud();

  virtual void draw();
  virtual void close();

  bool load_texture(const char *asset_name, const char *asset_path,
                    const int num_frames, const int is_player);
  bool load_textures();

  float get_global_scale();

  entity_id spawn_entity(const char *texture_key, float x, float y,
                         sprite_type type, bool is_anim, float scale);
  entity_id spawn_entity(const char *texture_key, float x, float y,
                         sprite_type type, bool is_anim);

  // unordered_map<entity_id, Vector2> &get_stars();
  unordered_map<entity_id, shared_ptr<Sprite>> &get_sprites();
  unordered_map<entity_id, shared_ptr<Sprite>> &get_bgsprites();
  unordered_map<string, texture_info> &get_textures();

  shared_ptr<Sprite> get_sprite(entity_id id);

  Camera2D &get_camera2d();
  Font &get_global_font();

  unsigned int get_current_frame();
  control_mode get_control_mode();

  vector<entity_id> &get_entity_ids();
  vector<entity_id> &get_bg_entity_ids();

  scene_transition get_scene_transition();
  scene_id get_id();
  scene_type get_scene_type();

  void set_camera_default_values();
  void set_debug_panel_on(bool b);
  void set_global_scale(float s);
  void set_control_mode(control_mode cm);
  void flip_debug_panel();
  void load_fonts();
  void set_texture_filepath(const char *filepath);
  // void add_star();
  void update_stars_vx(const float vx);
  void set_has_been_initialized(bool b);
  void set_scene_transition(scene_transition st);
  void set_alpha(float a);
  void set_id(scene_id i);
  void set_scene_type(scene_type st);
  void pause();
  void unpause();

  const bool get_has_been_initialized() const;
  const bool get_paused() const;
  const float get_alpha() const;
  const bool get_debug_panel_on() const;

  Mix_Music *get_music();
  void load_music(const char *path);
  void set_music_path(const char *path);
  const string get_music_path() const;

  const bool get_hud_on() const;
  void set_hud_on(const bool b);

  void incr_current_frame();

  shared_ptr<PopupManager> get_popup_manager();
  void set_popup_manager(shared_ptr<PopupManager> pm);

private:
  unordered_map<string, texture_info> textures;
  unordered_map<entity_id, shared_ptr<Sprite>> sprites;
  unordered_map<entity_id, shared_ptr<Sprite>> bgsprites;

  vector<entity_id> entity_ids;
  vector<entity_id> bg_entity_ids;

  Font global_font;

  Camera2D camera2d = {0};

  Mix_Music *music = NULL;

  float global_scale = 1.0f;
  float alpha = 1.0f;

  bool has_been_initialized = false;
  bool debug_panel_on = true;
  bool is_paused = false;
  bool hud_on = true;

  control_mode controlmode = CONTROL_MODE_PLAYER;

  unsigned int current_frame = 0;

  string texture_filepath;
  string music_path;

  // unordered_map<entity_id, Vector2> stars;

  scene_transition transition = SCENE_TRANSITION_NONE;

  scene_id id;

  scene_type scenetype;

  shared_ptr<PopupManager> popup_manager;
};
