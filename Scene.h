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
static entity_id next_entity_id = 0;

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

  string texture_filepath;
  string music_path;

  bool has_been_initialized = false;
  bool debug_panel_on = true;
  bool is_paused = false;
  bool hud_on = true;

  control_mode controlmode = CONTROL_MODE_PLAYER;

  unsigned int current_frame = 0;
  unsigned int global_font_size = 20;

  scene_transition transition = SCENE_TRANSITION_NONE;

  scene_id id;

  scene_type scenetype;

  shared_ptr<PopupManager> popup_manager;

public:
  Scene()
      : texture_filepath(""), debug_panel_on(true),
        controlmode(CONTROL_MODE_PLAYER) {}
  ~Scene() {}

  virtual void update() {}

  virtual bool init() { return true; }

  virtual void handle_input() {}

  virtual void draw_debug_panel() {}

  virtual void cleanup() {}

  virtual void draw_hud() {}

  virtual void draw() {}

  virtual void close() {
    for (auto &t : textures)
      UnloadTexture(t.second->texture);
    textures.clear();
    sprites.clear();
    entity_ids.clear();
    UnloadFont(global_font);
    if (music != NULL) {
      Mix_PauseMusic();
      Mix_FreeMusic(music);
    }
    has_been_initialized = false;
  }

  bool load_textures() {

    FILE *file = nullptr;
    char line[256];
    char *asset_name = nullptr, *num_frames = nullptr, *contexts = nullptr,
         *is_player = nullptr, *width = nullptr, *height = nullptr,
         *asset_path = nullptr;
    bool retval = false;
    if (texture_filepath == "") {
      mPrint("Error: texture_filepath not set.");
    } else {
      file = fopen(texture_filepath.c_str(), "r");
      if (file == NULL) {
        mPrint("Error opening file: " + texture_filepath);
      } else {
        while (fgets(line, sizeof(line), file)) {
          // if the line begins with a hashtag, it is a comment
          if (line[0] == '#') {
            continue;
          }
          asset_name = strtok(line, " ");
          num_frames = strtok(NULL, " ");
          contexts = strtok(NULL, " ");
          is_player = strtok(NULL, " ");
          width = strtok(NULL, " ");
          height = strtok(NULL, " ");
          asset_path = strtok(NULL, " ");
          // check to see if asset path has newline at end
          if (asset_path[strlen(asset_path) - 1] == '\n') {
            asset_path[strlen(asset_path) - 1] = '\0';
          }
          bool result = load_texture(asset_name, asset_path, atoi(num_frames),
                                     atoi(contexts), atoi(is_player),
                                     atoi(width), atoi(height));
          if (!result) {
            mPrint("Error loading texture: " + string(asset_path));
          }
        }
        fclose(file);
        retval = true;
      }
    }
    return retval;
  }

  bool load_texture(const char *asset_name, const char *asset_path,
                    const int num_frames, const int contexts,
                    const int is_player, const int width, const int height) {

    string asset_name_str = string(asset_name);
    Texture2D t = LoadTexture(asset_path);
    texture_info tx;
    if (t.id == 0) {
      mPrint("Error loading texture: " + string(asset_path));
      return false;
    }
    if (asset_name_str.find("tile-") != string::npos) {
      //  apply custom dithering
      Image img = LoadImageFromTexture(t);
      ImageDither(&img, 4, 4, 4, 4);
      Texture2D tt = LoadTextureFromImage(img);
      UnloadImage(img);
      t = tt;
    }
    tx.texture = t;
    tx.num_frames = num_frames;
    tx.contexts = contexts;
    tx.is_player = is_player;
    tx.asset_path = asset_path;
    tx.width = width;
    tx.height = height;
    textures[asset_name] = make_shared<texture_info>(tx);
    return true;
  }

  entity_id spawn_entity(const char *key, const float x, const float y,
                         const sprite_type type, const bool is_anim,
                         const float scale) {
    if (textures.find(key) == textures.end()) {
      mPrint("Error: texture not found: " + string(key));
      return -1;
    }

    shared_ptr<Sprite> s =
        make_shared<Sprite>(textures[key]->texture, textures[key]->num_frames,
                            textures[key]->contexts, x, y, textures[key]->width,
                            textures[key]->height, type);
    if (s == nullptr) {
      mPrint("Error creating sprite.");
      return -1;
    }
    s->set_scale(scale);
    s->set_is_animating(is_anim);
    sprites[next_entity_id] = s;
    entity_ids.push_back(next_entity_id);
    return next_entity_id++;
  }

  entity_id spawn_entity(const char *key, const float x, const float y,
                         const sprite_type type, const bool is_anim) {
    return spawn_entity(key, x, y, type, is_anim, global_scale);
  }

  shared_ptr<texture_info> get_texture_info(const string key) {
    if (textures.find(key) == textures.end()) {
      mPrint("Error: texture not found: " + key);
      return nullptr;
    }
    return textures[key];
  }

  void set_camera_default_values() {
    camera2d.target.x = camera2d.target.y = camera2d.offset.x =
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

  void load_music(const char *path) {
    music = Mix_LoadMUS(path);
    if (music == NULL) {
      string err = Mix_GetError();
      mPrint("Mix_LoadMUS: " + err);
      return;
    }
  }

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
