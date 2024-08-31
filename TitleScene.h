#pragma once

#include "Entity.h"
#include "PopupManager.h"
#include "SceneTransition.h"
#include "SceneType.h"
#include "Scene_id.h"
#include "Sprite.h"
#include "TextureInfo.h"
#include "control_mode.h"
#include "mPrint.h"

#include "raylib.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

typedef struct {
    RenderTexture prerendered_texture;
    Rectangle src_rect;
    Rectangle dst_rect;
    Vector2 origin = (Vector2){0, 0};
    unordered_map<string, shared_ptr<TextureInfo>> textures;
    unordered_map<EntityId, shared_ptr<Sprite>> sprites;
    unordered_map<EntityId, shared_ptr<Entity>> entities;
    vector<EntityId> entity_ids;
    Font global_font;
    Camera2D camera2d = {0};
    Mix_Music* music = NULL;
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
    SceneTransition transition = SCENE_TRANSITION_NONE;
    Scene_id id;
    SceneType scenetype;
    //shared_ptr<PopupManager> popup_manager;
    PopupManager popup_manager;

} TitleScene;

void TitleScene_create(TitleScene& t);
void TitleScene_set_control_mode(TitleScene& t, control_mode mode);
void TitleScene_set_texture_filepath(TitleScene& t, string filepath);
void TitleScene_set_scene_transition(TitleScene& t, SceneTransition transition);
void TitleScene_set_scene_type(TitleScene& t, SceneType type);
void TitleScene_destroy(TitleScene& t);
bool TitleScene_init(TitleScene& t);
void TitleScene_draw(TitleScene& t);
void TitleScene_handle_input(TitleScene& t);
void TitleScene_draw_debug_panel(TitleScene& t);
void TitleScene_close(TitleScene& t);
void TitleScene_cleanup(TitleScene& t);
void TitleScene_update(TitleScene& t);
void TitleScene_prerender_texture(TitleScene& t);
bool TitleScene_get_has_been_initialized(TitleScene& t);
bool TitleScene_get_debug_panel_on(TitleScene& t);
void TitleScene_set_has_been_initialized(TitleScene& t, bool value);
void TitleScene_flip_debug_panel(TitleScene& t);
void TitleScene_set_camera_default_values(TitleScene& t);

control_mode TitleScene_get_control_mode(TitleScene& t);
string TitleScene_get_texture_filepath(TitleScene& t);
SceneTransition TitleScene_get_scene_transition(TitleScene& t);
SceneType TitleScene_get_scene_type(TitleScene& t);

bool TitleScene_load_texture(TitleScene& t,
                             const char* asset_name,
                             const char* asset_path,
                             const int num_frames,
                             const int contexts,
                             const int is_player,
                             const int width,
                             const int height);

bool TitleScene_load_textures(TitleScene& t);

void TitleScene_incr_current_frame(TitleScene& t);

Font& TitleScene_get_global_font(TitleScene& t);
//void TitleScene_set_popup_manager(TitleScene& t, shared_ptr<PopupManager> pm);
//void TitleScene_set_popup_manager(TitleScene& t, PopupManager& pm);

void TitleScene_set_id(TitleScene& t, Scene_id id);
Scene_id TitleScene_get_id(TitleScene& t);

float TitleScene_get_alpha(TitleScene& t);
void TitleScene_set_alpha(TitleScene& t, float a);

void TitleScene_set_scene_transition(TitleScene& t, SceneTransition transition);
