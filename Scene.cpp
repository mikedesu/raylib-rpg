#include "Scene.h"
#include "mPrint.h"
#include "rlgl.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <cassert>
#include <cstring>
#include <raylib.h>

static entity_id next_entity_id = 0;

Scene::Scene() {
  mPrint("Scene constructor");
  controlmode = CONTROL_MODE_PLAYER;
  texture_filepath = "";
  debug_panel_on = true;
}

void Scene::set_texture_filepath(const char *filepath) {
  texture_filepath = filepath;
}

Scene::~Scene() { mPrint("Scene destructor"); }

void Scene::close() {
  mPrint("Closing scene...");
  mPrint("Unloading textures...");
  for (auto &t : textures) {
    UnloadTexture(t.second->texture);
  }
  mPrint("Clearing textures...");
  textures.clear();
  mPrint("Clearing sprites...");
  sprites.clear();
  mPrint("Clearing entity ids...");
  entity_ids.clear();
  mPrint("Unloading font...");
  UnloadFont(global_font);

  if (music != NULL) {
    // stop music
    Mix_PauseMusic();
    Mix_FreeMusic(music);
  }

  has_been_initialized = false;

  mPrint("Scene closed.");
}

void Scene::draw() {}
void Scene::update() {}
void Scene::handle_input() {}
void Scene::draw_debug_panel() {}
void Scene::draw_hud() {}
void Scene::cleanup() {}
bool Scene::init() { return true; }

bool Scene::load_textures() {
  mPrint("Loading textures...");
  // check if the file exists
  if (texture_filepath == "") {
    mPrint("Error: texture_filepath not set.");
    return false;
  }
  FILE *file = fopen(texture_filepath.c_str(), "r");
  if (file == NULL) {
    mPrint("Error opening file: " + texture_filepath);
    return false;
  }
  char line[256];
  while (fgets(line, sizeof(line), file)) {
    // if the line begins with a hashtag, it is a comment and does not get
    // processed
    if (line[0] == '#') {
      continue;
    }

    char *asset_name = strtok(line, " ");
    char *num_frames = strtok(NULL, " ");
    char *contexts = strtok(NULL, " ");
    char *is_player = strtok(NULL, " ");
    char *width = strtok(NULL, " ");
    char *height = strtok(NULL, " ");
    char *asset_path = strtok(NULL, " ");
    mPrint("Asset name: " + string(asset_name));
    mPrint("Num frames: " + string(num_frames));
    mPrint("Contexts: " + string(contexts));
    mPrint("Is player: " + string(is_player));
    mPrint("Width: " + string(width));
    mPrint("Height: " + string(height));
    mPrint("Asset path: [" + string(asset_path) + "]");

    // check to see if asset path has newline at end
    if (asset_path[strlen(asset_path) - 1] == '\n') {
      asset_path[strlen(asset_path) - 1] = '\0';
    }

    bool result =
        load_texture(asset_name, asset_path, atoi(num_frames), atoi(contexts),
                     atoi(is_player), atoi(width), atoi(height));
    if (!result) {
      mPrint("Error loading texture: " + string(asset_path));
      return false;
    }
  }
  fclose(file);
  return true;
}

bool Scene::load_texture(const char *asset_name, const char *asset_path,
                         const int num_frames, const int contexts,
                         const int is_player, const int width,
                         const int height) {

  string asset_name_str = string(asset_name);
  mPrint("Attempting to load texture: " + asset_name_str);

  // if the asset name begins with "tile-", then it is assumed to be a tile
  // we want to experiment with applying custom dithering to the loaded texture
  // before storing it in the textures map for later use
  //
  // we might even consider converting what we store in the textures map
  // currently its string->texture_info
  // but we could make it string->vector<texture_info>
  // which would allow for us to store a number of different texture_info
  // objects for each asset name this would require a little refactoring for
  // spawning entities and drawing the tiles but it shouldn't be too bad

  Texture2D t = LoadTexture(asset_path);
  texture_info ti;

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

  ti.texture = t;
  ti.num_frames = num_frames;
  ti.contexts = contexts;
  ti.is_player = is_player;
  ti.asset_path = asset_path;
  ti.width = width;
  ti.height = height;

  textures[asset_name] = make_shared<texture_info>(ti);

  return true;
}

void Scene::set_camera_default_values() {
  camera2d.target.x = 0;
  camera2d.target.y = 0;
  camera2d.offset.x = 0;
  camera2d.offset.y = 0;
  camera2d.rotation = 0.0f;
  camera2d.zoom = 1;
}

void Scene::set_global_scale(float s) {
  assert(s > 0);
  global_scale = s;
}

void Scene::load_fonts() {
  mPrint("Loading fonts...");
  const unsigned int global_font_size = 20;
  const char font_path[] = "fonts/hack.ttf";
  // global_font = LoadFont(font_path);
  global_font = LoadFontEx(font_path, global_font_size, 0, 250);
}

entity_id Scene::spawn_entity(const char *texture_key, float x, float y,
                              sprite_type type, bool is_anim) {
  return spawn_entity(texture_key, x, y, type, is_anim, global_scale);
}

entity_id Scene::spawn_entity(const char *texture_key, float x, float y,
                              sprite_type type, bool is_anim, float scale) {

  const int w = textures[texture_key]->width;
  const int h = textures[texture_key]->height;
  const int num_frames = textures[texture_key]->num_frames;
  const int contexts = textures[texture_key]->contexts;

  mPrint("Spawning entity: " + string(texture_key) + " at (" + to_string(x) +
         ", " + to_string(y) + ") with width: " + to_string(w) +
         " and height: " + to_string(h));

  shared_ptr<Sprite> s = make_shared<Sprite>(
      textures[texture_key]->texture, num_frames, contexts, x, y, w, h, type);
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

const bool Scene::get_has_been_initialized() const {
  return has_been_initialized;
}

unordered_map<entity_id, shared_ptr<Sprite>> &Scene::get_sprites() {
  return sprites;
}

unordered_map<string, shared_ptr<texture_info>> &Scene::get_textures() {
  return textures;
}

Camera2D &Scene::get_camera2d() { return camera2d; }
vector<entity_id> &Scene::get_entity_ids() { return entity_ids; }
float Scene::get_global_scale() { return global_scale; }
scene_transition Scene::get_scene_transition() { return transition; }
scene_id Scene::get_id() { return id; }
unsigned int Scene::get_current_frame() { return current_frame; }
control_mode Scene::get_control_mode() { return controlmode; }
Font &Scene::get_global_font() { return global_font; }
scene_type Scene::get_scene_type() { return scenetype; }

const bool Scene::get_debug_panel_on() const { return debug_panel_on; }
const float Scene::get_alpha() const { return alpha; }
const bool Scene::get_paused() const { return is_paused; }
const string Scene::get_music_path() const { return music_path; }
const bool Scene::get_hud_on() const { return hud_on; }

Mix_Music *Scene::get_music() { return music; }

void Scene::set_scene_transition(scene_transition st) { transition = st; }
void Scene::set_scene_type(scene_type st) { scenetype = st; }
void Scene::set_alpha(float a) { alpha = a; }
void Scene::set_id(scene_id i) { id = i; }
void Scene::pause() { is_paused = true; }
void Scene::unpause() { is_paused = false; }
void Scene::set_music_path(const char *path) { music_path = path; }
void Scene::set_hud_on(const bool b) { hud_on = b; }
void Scene::incr_current_frame() { current_frame++; }
void Scene::set_debug_panel_on(bool b) { debug_panel_on = b; }
void Scene::flip_debug_panel() { debug_panel_on = !debug_panel_on; }
void Scene::set_control_mode(control_mode cm) { controlmode = cm; }
void Scene::set_has_been_initialized(bool b) { has_been_initialized = b; }

shared_ptr<Sprite> Scene::get_sprite(entity_id id) { return sprites[id]; }
shared_ptr<PopupManager> Scene::get_popup_manager() { return popup_manager; }

void Scene::load_music(const char *path) {
  music = Mix_LoadMUS(path);
  if (music == NULL) {
    string err = Mix_GetError();
    mPrint("Mix_LoadMUS: " + err);
    return;
  }
}

void Scene::set_popup_manager(shared_ptr<PopupManager> pm) {
  popup_manager = pm;
}

shared_ptr<texture_info> Scene::get_texture_info(const string key) {
  if (textures.find(key) == textures.end()) {
    mPrint("Error: texture not found: " + key);
    return nullptr;
  }
  return textures[key];
}

shared_ptr<Entity> Scene::get_entity(entity_id id) { return entities[id]; }

const unsigned int Scene::get_global_font_size() const {
  return global_font.baseSize;
}
void Scene::set_global_font_size(const unsigned int size) {
  global_font.baseSize = size;
}
