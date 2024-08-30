#pragma once

#include "CurrentScene.h"
#include "GameplayScene.h"
#include "PopupManager.h"
#include "Sprite.h"
#include "TitleScene.h"
#include "control_mode.h"
#include "entity_id.h"
#include "texture_info.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

typedef struct {
    //unordered_map<string, texture_info> textures;
    //unordered_map<entity_id, shared_ptr<Sprite>> sprites;
    //unordered_map<entity_id, bool> gravity;

    //unordered_map<Scene_id, shared_ptr<Scene>> scenes;

    unordered_map<string, Scene_id> scene_keys;
    vector<entity_id> entity_ids;
    //shared_ptr<Scene> current_scene;
    shared_ptr<PopupManager> popup_manager;
    entity_id player_id;
    Scene_id current_scene_id;
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

    TitleScene title_scene;
    GameplayScene gameplay_scene;
    //shared_ptr<GameplayScene> gameplay_scene;

    CurrentScene current_scene = SCENE_TITLE;

} Game;

void Game_create(Game& g);
bool Game_init_audio();
bool Game_init(Game& g);

void Game_destroy(Game& g);

inline void Game_update(Game& g);
inline void Game_handle_input(Game& g);
inline void Game_cleanup(Game& g);
inline void Game_draw(Game& g);
void Game_run(Game& g);
void Game_handle_transition_in(Game& g);
void Game_handle_transition_out(Game& g);
void Game_draw_debug_panel(Game& g);
void Game_load_fonts(Game& g);
void Game_set_global_scale(Game& game, float s);
void Game_set_debug_panel(Game& game, bool b);
void Game_set_screen_width(Game& game, int w);
void Game_set_screen_height(Game& game, int h);
void Game_set_window_title(Game& game, const char* t);
void Game_set_camera_default_values(Game& g);
void Game_set_has_been_initialized(Game& game, bool b);
void Game_close(Game& g);
void Game_spawn_scenes(Game& g);
bool Game_get_has_been_initialized(Game& g);
string Game_get_window_title(Game& g);

//class Game {
//private:
//    unordered_map<string, texture_info> textures;
//    unordered_map<entity_id, shared_ptr<Sprite>> sprites;
//    unordered_map<entity_id, bool> gravity;
//    unordered_map<scene_id, shared_ptr<Scene>> scenes;
//    unordered_map<string, scene_id> scene_keys;
//    vector<entity_id> entity_ids;
//    shared_ptr<Scene> current_scene;
//    shared_ptr<PopupManager> popup_manager;
//    entity_id player_id;
//    scene_id current_scene_id;
//    bool has_been_initialized;
//    bool debug_panel_on;
//    float global_scale;
//    unsigned int current_frame;
//    control_mode controlmode;
//    string window_title;
//    Font global_font;
//    Camera2D camera2d;
//    RenderTexture target;
//    Rectangle screen_rect;
//public:
//    Game();
//    ~Game();
//    inline void update();
//    inline void handle_input();
//    inline void cleanup();
//    inline void draw();
//    void run();
//    void handle_transition_in();
//    void handle_transition_out();
//    void draw_debug_panel();
//    void load_fonts();
//    void set_global_scale(float s);
//    void set_debug_panel(bool b);
//    void set_screen_width(int w);
//    void set_screen_height(int h);
//    void set_window_title(const char* t);
//    void set_camera_default_values();
//    void set_has_been_initialized(bool b);
//    void close();
//    void spawn_scenes();
//    bool get_has_been_initialized();
//    bool init();
//    bool init_audio();
//    string get_window_title();
//};
