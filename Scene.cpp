#include "Scene.h"
#include "mPrint.h"
#include "rlgl.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <cstring>
#include <raylib.h>

static entity_id next_entity_id = 0;

Scene::Scene() {
  mPrint("Scene constructor");
  controlmode = CONTROL_MODE_PLAYER;
  texture_filepath = "";
  debug_panel_on = true;
}

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
    // if the line begins with a hashtag, it is a comment
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

entity_id Scene::spawn_entity(const char *texture_key, float x, float y,
                              sprite_type type, bool is_anim, float scale) {
  const int w = textures[texture_key]->width;
  const int h = textures[texture_key]->height;
  const int num_frames = textures[texture_key]->num_frames;
  const int contexts = textures[texture_key]->contexts;
  mPrint("Spawning entity: " + string(texture_key) + " at (" + to_string(x) +
         ", " + to_string(y) + ") with width: " + to_string(w) +
         " and height: " + to_string(h) + " and num_frames: " +
         to_string(num_frames) + " and contexts: " + to_string(contexts) +
         " and type: " + to_string(type) + " and is_anim: " +
         to_string(is_anim) + " and scale: " + to_string(scale));
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

void Scene::load_music(const char *path) {
  music = Mix_LoadMUS(path);
  if (music == NULL) {
    string err = Mix_GetError();
    mPrint("Mix_LoadMUS: " + err);
    return;
  }
}

shared_ptr<texture_info> Scene::get_texture_info(const string key) {
  if (textures.find(key) == textures.end()) {
    mPrint("Error: texture not found: " + key);
    return nullptr;
  }
  return textures[key];
}

entity_id Scene::spawn_entity(const char *texture_key, float x, float y,
                              sprite_type type, bool is_anim) {
  return spawn_entity(texture_key, x, y, type, is_anim, global_scale);
}
Scene::~Scene() { mPrint("Scene destructor"); }
void Scene::draw() {}
void Scene::update() {}
void Scene::handle_input() {}
void Scene::draw_debug_panel() {}
void Scene::draw_hud() {}
void Scene::cleanup() {}
bool Scene::init() { return true; }
