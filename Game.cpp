#include "Game.h"
#include "mPrint.h"
#include "raylib.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <cassert>

static scene_id next_scene_id = 0;

using namespace std;

void Game_create(Game& g) {
    g.global_scale = 1.0f;
    g.screen_rect = (Rectangle){0, 0, 1920, -1080};
    g.camera2d = {0};
    g.current_frame = 0;
    Game_set_camera_default_values(g);
    g.has_been_initialized = false;
    g.controlmode = CONTROL_MODE_PLAYER;
}

bool Game_init_audio() {
    if(SDL_Init(SDL_INIT_AUDIO) < 0) {
        mPrint("SDL2 audio could not be initialized. Exiting...");
        return false;
    }
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        mPrint("SDL2 audio could not be initialized. Exiting...");
        return false;
    }
    return true;
}

bool Game_init(Game& g) {
    if(!g.has_been_initialized) {
        InitWindow(g.screen_rect.width, -g.screen_rect.height, Game_get_window_title(g).c_str());
        // init SDL2 for audio
        if(!Game_init_audio()) {
            return false;
        }
        Game_set_camera_default_values(g);
        SetTargetFPS(60); // Set our game to run at 60 frames-per-second
        g.popup_manager = make_shared<PopupManager>();
        Game_spawn_scenes(g);
        for(auto& scene : g.scenes) {
            mPrint("Initializing scenes...");
            bool result = scene.second->init();
            if(!result) {
                mPrint("Error loading scene. Exiting...");
                return false;
            }
        }
        // set the current scene
        string key = "title";
        // string key = "gameplay";
        if(g.scene_keys.find(key) == g.scene_keys.end()) {
            mPrint("Error: " + key + " scene not found.");
            return false;
        }
        g.current_scene_id = g.scene_keys[key];
        g.target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
        SetExitKey(KEY_Q);
        SetRandomSeed(GetTime());
        g.has_been_initialized = true;
    }
    return true;
}

void Game_spawn_scenes(Game& g) {
    shared_ptr<Scene> title_scene = make_shared<TitleScene>();
    title_scene->set_id(next_scene_id++);
    title_scene->set_popup_manager(g.popup_manager);
    g.scenes[title_scene->get_id()] = title_scene;
    g.scene_keys["title"] = title_scene->get_id();

    shared_ptr<Scene> gameplay_scene = make_shared<GameplayScene>();
    gameplay_scene->set_id(next_scene_id++);
    gameplay_scene->set_popup_manager(g.popup_manager);
    g.scenes[gameplay_scene->get_id()] = gameplay_scene;
    g.scene_keys["gameplay"] = gameplay_scene->get_id();

    //shared_ptr<Scene> gameover_scene = make_shared<GameoverScene>();
    //gameover_scene->set_id(next_scene_id++);
    //gameover_scene->set_popup_manager(g.popup_manager);
    //g.scenes[gameover_scene->get_id()] = gameover_scene;
    //g.scene_keys["gameover"] = gameover_scene->get_id();
}

void Game_set_camera_default_values(Game& g) {
    g.camera2d.target.x = 0;
    g.camera2d.target.y = 0;
    g.camera2d.offset.x = 0;
    g.camera2d.offset.y = 0;
    g.camera2d.rotation = 0.0f;
    g.camera2d.zoom = 1;
}

void Game_destroy(Game& g) {
    Game_close(g);
}

string Game_get_window_title(Game& g) {
    return g.window_title;
}

void Game_set_window_title(Game& g, const char* title) {
    g.window_title = title;
}

void Game_set_screen_width(Game& g, int w) {
    g.screen_rect.width = w;
}

void Game_set_screen_height(Game& g, int h) {
    g.screen_rect.height = -h;
}

void Game_set_debug_panel(Game& g, bool b) {
    g.debug_panel_on = b;
}

void Game_set_has_been_initialized(Game& g, bool b) {
    g.has_been_initialized = b;
}

bool Game_get_has_been_initialized(Game& g) {
    return g.has_been_initialized;
}

inline void Game_handle_input(Game& g) {
    g.scenes[g.current_scene_id]->handle_input();
}

inline void Game_update(Game& g) {
    g.scenes[g.current_scene_id]->update();
}

inline void Game_cleanup(Game& g) {
    g.scenes[g.current_scene_id]->cleanup();
}

void Game_set_global_scale(Game& g, float s) {
    assert(s > 0.0f);
    g.global_scale = s;
}

void Game_load_fonts(Game& g) {
    mPrint("Loading fonts...");
    const char font_path[] = "fonts/hack.ttf";
    g.global_font = LoadFont(font_path);
}

void Game_handle_transition_out(Game& g) {
    const Color c = (Color){0x66, 0x66, 0x66};
    const float a = g.scenes[g.current_scene_id]->get_alpha();
    const int w = GetScreenWidth();
    const int h = GetScreenHeight();
    // const float transition_speed = 0.020f;
    const float transition_speed = 0.040f;

    if(a < 1.0f) {
        DrawRectangle(0, 0, w, h, Fade(c, a));
        g.scenes[g.current_scene_id]->set_alpha(g.scenes[g.current_scene_id]->get_alpha() +
                                                transition_speed);
    } else {
        DrawRectangle(0, 0, w, h, Fade(c, a));

        g.scenes[g.current_scene_id]->close();

        // this makes the assumption that we will ALWAYS transition into the
        // gameplay scene and that we are on the title scene this is untrue!

        const scene_id title_scene_id = g.scene_keys["title"];
        const scene_id gameplay_scene_id = g.scene_keys["gameplay"];
        const scene_id gameover_scene_id = g.scene_keys["gameover"];

        if(g.scenes[title_scene_id]->get_has_been_initialized() == false) {
            g.scenes[title_scene_id]->init();
        }
        if(g.scenes[gameover_scene_id]->get_has_been_initialized() == false) {
            g.scenes[gameover_scene_id]->init();
        }
        if(g.scenes[gameplay_scene_id]->get_has_been_initialized() == false) {
            g.scenes[gameplay_scene_id]->init();
        }

        if(g.current_scene_id == title_scene_id) {
            g.scenes[gameplay_scene_id]->set_scene_transition(SCENE_TRANSITION_IN);
            g.current_scene_id = gameplay_scene_id;
        } else if(g.current_scene_id == gameplay_scene_id) {
            g.scenes[gameover_scene_id]->set_scene_transition(SCENE_TRANSITION_IN);
            g.current_scene_id = gameover_scene_id;
        } else if(g.current_scene_id == gameover_scene_id) {
            g.scenes[title_scene_id]->set_scene_transition(SCENE_TRANSITION_IN);
            g.current_scene_id = title_scene_id;
        }
    }
}

void Game_handle_transition_in(Game& g) {
    const Color c = (Color){0x66, 0x66, 0x66};
    const float a = g.scenes[g.current_scene_id]->get_alpha();
    const int w = GetScreenWidth();
    const int h = GetScreenHeight();
    const float transition_speed = 0.040f;
    // const float transition_speed = 0.001f;

    if(a > 0.0f) {
        DrawRectangle(0, 0, w, h, Fade(c, a));
        g.scenes[g.current_scene_id]->set_alpha(g.scenes[g.current_scene_id]->get_alpha() -
                                                transition_speed);
    } else {
        g.scenes[g.current_scene_id]->set_scene_transition(SCENE_TRANSITION_NONE);
    }
}

inline void Game_draw(Game& g) {
    BeginDrawing();
    BeginTextureMode(g.target);
    ClearBackground(BLACK);
    g.scenes[g.current_scene_id]->draw();
    EndTextureMode();
    DrawTextureRec(g.target.texture, g.screen_rect, (Vector2){0, 0}, WHITE);
    switch(g.scenes[g.current_scene_id]->get_scene_transition()) {
    case SCENE_TRANSITION_NONE:
        break;
    case SCENE_TRANSITION_IN:
        Game_handle_transition_in(g);
        break;
    case SCENE_TRANSITION_OUT:
        Game_handle_transition_out(g);
        break;
    default:
        break;
    }
    EndDrawing();
    g.current_frame++;
}

void Game_run(Game& g) {
    if(!g.has_been_initialized) {
        mPrint("Game has not been initialized. Exiting...");
    } else {
        while(!WindowShouldClose()) {
            Game_handle_input(g);
            Game_update(g);
            Game_draw(g);
            Game_cleanup(g);
        }
        mPrint("Window closed.");
    }
}

void Game_close(Game& g) {
    mPrint("Closing game...");
    // have to close the scene first otherwise it crashes
    // CloseWindow() must be the LAST raylib call before exiting
    g.scenes[g.current_scene_id]->close();
    if(IsRenderTextureReady(g.target)) {
        mPrint("Unloading render texture...");
        UnloadRenderTexture(g.target);
    }
    mPrint("Closing SDL2 audio...");
    Mix_CloseAudio();
    mPrint("Quitting SDL2 audio...");
    SDL_Quit();
    if(IsWindowReady()) {
        mPrint("Closing window...");
        CloseWindow();
    }
    mPrint("Game closed.");
}
