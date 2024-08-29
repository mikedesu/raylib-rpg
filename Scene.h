#pragma once

#include "Entity.h"
#include "PopupManager.h"
#include "Sprite.h"
#include "control_mode.h"
#include "entity_id.h"
#include "mPrint.h"
#include "raylib.h"
#include "texture_info.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

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

private:
  unordered_map<string, shared_ptr<texture_info>> textures;
  unordered_map<entity_id, shared_ptr<Sprite>> sprites;

  vector<entity_id> entity_ids;
  unordered_map<entity_id, shared_ptr<Entity>> entities;

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
  unsigned int global_font_size = 20;

  string texture_filepath;
  string music_path;

  scene_transition transition = SCENE_TRANSITION_NONE;

  scene_id id;

  scene_type scenetype;

  shared_ptr<PopupManager> popup_manager;

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

  bool load_textures();
  bool load_texture(const char *asset_name, const char *asset_path,
                    const int num_frames, const int contexts,
                    const int is_player, const int width, const int height);

  entity_id spawn_entity(const char *texture_key, float x, float y,
                         sprite_type type, bool is_anim, float scale);
  entity_id spawn_entity(const char *texture_key, float x, float y,
                         sprite_type type, bool is_anim);

  shared_ptr<texture_info> get_texture_info(const string key);

  void set_camera_default_values() {
    camera2d.target.x = 0;
    camera2d.target.y = 0;
    camera2d.offset.x = 0;
    camera2d.offset.y = 0;
    camera2d.rotation = 0.0f;
    camera2d.zoom = 1;
  }
  void set_debug_panel_on(bool b) { debug_panel_on = b; }
  void set_global_scale(float s) { global_scale = s; }
  void set_control_mode(control_mode cm) { controlmode = cm; }
  void flip_debug_panel() { debug_panel_on = !debug_panel_on; }
  void load_fonts() {
    global_font = LoadFontEx("fonts/hack.ttf", global_font_size, 0, 250);
  }
  void set_texture_filepath(const char *filepath) {
    texture_filepath = filepath;
  }
  void set_has_been_initialized(bool b) { has_been_initialized = b; }
  void set_scene_transition(scene_transition st) { transition = st; }
  void set_alpha(float a) { alpha = a; }
  void set_id(scene_id i) { id = i; }
  void set_scene_type(scene_type st) { scenetype = st; }
  void pause() { is_paused = true; }
  void unpause() { is_paused = false; }
  void load_music(const char *path);
  void set_music_path(const char *path) { music_path = path; }
  void incr_current_frame() { current_frame++; }
  void set_hud_on(const bool b) { hud_on = b; }
  void set_popup_manager(shared_ptr<PopupManager> pm) { popup_manager = pm; }

  unordered_map<string, shared_ptr<texture_info>> &get_textures() {
    return textures;
  }
  unordered_map<entity_id, shared_ptr<Entity>> &get_entities() {
    return entities;
  }
  unordered_map<entity_id, shared_ptr<Sprite>> &get_sprites() {
    return sprites;
  }
  shared_ptr<Sprite> get_sprite(entity_id id) { return sprites[id]; }
  unsigned int get_current_frame() { return current_frame; }
  control_mode get_control_mode() { return controlmode; }
  vector<entity_id> &get_entity_ids() { return entity_ids; }
  shared_ptr<PopupManager> get_popup_manager() { return popup_manager; }
  scene_transition get_scene_transition() { return transition; }
  scene_id get_id() { return id; }
  scene_type get_scene_type() { return scenetype; }
  Camera2D &get_camera2d() { return camera2d; }
  Font &get_global_font() { return global_font; }
  const bool get_has_been_initialized() const { return has_been_initialized; }
  const bool get_paused() const { return is_paused; }
  const float get_alpha() const { return alpha; }
  const bool get_debug_panel_on() const { return debug_panel_on; }
  const string get_music_path() const { return music_path; }
  const bool get_hud_on() const { return hud_on; }

  Mix_Music *get_music() { return music; }

  const unsigned int get_global_font_size() const { return global_font_size; }
  void set_global_font_size(const unsigned int size) {
    global_font_size = size;
  }

  shared_ptr<Entity> get_entity(entity_id id) { return entities[id]; }
  float get_global_scale() { return global_scale; }
};
