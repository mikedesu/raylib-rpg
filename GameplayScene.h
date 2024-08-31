#pragma once

#include "DungeonEvent.h"
#include "DungeonFloor.h"
//#include "MessagePane.h"
#include "ControlMode.h"
#include "PopupManager.h"
#include "SceneId.h"
#include "SceneTransition.h"
#include "SceneType.h"
#include "TextureInfo.h"

#include <vector>

typedef struct {
    bool show_test_popup = true;
    EntityId player_id = -1;
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
    //shared_ptr<MessagePane> message_pane;
    // common to titlescene and gameplayscene
    ControlMode controlmode = CONTROL_MODE_PLAYER;
    string texture_filepath;
    float global_scale = 1.0f;
    SceneTransition transition = SCENE_TRANSITION_NONE;
    SceneType scenetype;
    unordered_map<EntityId, shared_ptr<Sprite>> sprites;
    unordered_map<string, shared_ptr<TextureInfo>> textures;
    Camera2D camera2d = {0};
    PopupManager popup_manager;
    bool debug_panel_on = true;
    bool has_been_initialized = false;
    Font global_font;
    bool is_paused = false;
    unsigned int current_frame = 0;
    vector<EntityId> entity_ids;
    bool hud_on = true;
    float alpha = 0.0f;
    unsigned int global_font_size = 20;
    SceneId id;

} GameplayScene;

void GameplayScene_create(GameplayScene& g);
void GameplayScene_destroy(GameplayScene& g);
bool GameplayScene_init(GameplayScene& g);
bool GameplayScene_handle_dungeon_move_pos(GameplayScene& g,
                                           const EntityId id,
                                           const Vector2 c_pos,
                                           const Vector2 t_pos);
void GameplayScene_update(GameplayScene& g);
void GameplayScene_handle_input(GameplayScene& g);
void GameplayScene_handle_control_mode_switch(GameplayScene& g);
void GameplayScene_close(GameplayScene& g);
void GameplayScene_cleanup(GameplayScene& g);
void GameplayScene_gameover(GameplayScene& g);
void GameplayScene_init_dungeon_floor(GameplayScene& g);
void GameplayScene_set_scale(GameplayScene& g, const float f);
void GameplayScene_increase_lighting_at(GameplayScene& g,
                                        const Vector2 loc,
                                        const int light_level,
                                        const int dist);
void GameplayScene_decrease_lighting_at(GameplayScene& g,
                                        const Vector2 loc,
                                        const int light_level,
                                        const int dist);
inline void GameplayScene_handle_player_input(GameplayScene& g);
inline void GameplayScene_handle_player_move_direction(GameplayScene& g);
inline void GameplayScene_handle_player_mouse_click(GameplayScene& g);
inline void GameplayScene_handle_camera_input(GameplayScene& g);
inline void GameplayScene_handle_camera_input_move(GameplayScene& g);
inline void GameplayScene_handle_camera_input_zoom(GameplayScene& g);
inline void GameplayScene_handle_tile_selection_input(GameplayScene& g);
inline void GameplayScene_draw_debug_panel(GameplayScene& g);
inline void GameplayScene_draw_hud(GameplayScene& g);
inline void GameplayScene_draw_message_log(GameplayScene& g);
inline void GameplayScene_draw_controls(GameplayScene& g);
void GameplayScene_draw(GameplayScene& g);
inline void
GameplayScene_draw_tile(GameplayScene& g, const string tile_key, const int i, const int j);
inline void GameplayScene_handle_popup_manager(GameplayScene& g);
inline void
GameplayScene_handle_tile_click(GameplayScene& g, const Rectangle dest, const int i, const int j);
Vector2
GameplayScene_handle_dungeon_move_dir(GameplayScene& g, const EntityId id, const Vector2 direction);
const string GameplayScene_tile_key_for_type(GameplayScene& g, const TileType t);
const Vector2 GameplayScene_get_start_location(GameplayScene& g);

const EntityId GameplayScene_spawn_player(GameplayScene& g, const Vector2 pos);
const EntityId GameplayScene_spawn_column(GameplayScene& g, const Vector2 pos);
const EntityId GameplayScene_spawn_torch(GameplayScene& g, const Vector2 pos);

void GameplayScene_remove_torch(GameplayScene& g, const Vector2 pos);
void GameplayScene_handle_spawn_column(GameplayScene& g, const Vector2 p);

void GameplayScene_set_camera_default_values(GameplayScene& g);

void GameplayScene_load_fonts(GameplayScene& g);

bool GameplayScene_load_textures(GameplayScene& g);

bool GameplayScene_load_texture(GameplayScene& g,
                                const char* asset_name,
                                const char* asset_path,
                                const int num_frames,
                                const int contexts,
                                const int is_player,
                                const int width,
                                const int height);

EntityId GameplayScene_spawn_entity(GameplayScene& g,
                                    const char* key,
                                    const float x,
                                    const float y,
                                    const SpriteType type,
                                    const bool is_anim,
                                    const float scale);

EntityId GameplayScene_spawn_entity(GameplayScene& g,
                                    const char* key,
                                    const float x,
                                    const float y,
                                    const SpriteType type,
                                    const bool is_anim);

void GameplayScene_set_id(GameplayScene& g, SceneId id);

//void GameplayScene_set_popup_manager(GameplayScene& t, shared_ptr<PopupManager> pm);
