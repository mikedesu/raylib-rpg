#include "Game.h"
#include "GameplayScene.h"
#include "mPrint.h"
#include "raylib.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <cassert>

static SceneId next_scene_id = 0;

using namespace std;

void Game_create(Game& g) {
    g.global_scale = 1.0f;
    //const int w = 1280;
    const int w = 1920;
    //const int h = 720;
    const int h = 1080;
    g.screen_rect = (Rectangle){0, 0, w, -h};
    //g.screen_rect = (Rectangle){0, 0, 1920, -1080};
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
        //g.popup_manager = make_shared<PopupManager>();
        PopupManager_create(g.popup_manager);

        Game_spawn_scenes(g);

        g.current_scene = SCENE_TITLE;
        g.target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
        SetExitKey(KEY_Q);
        SetRandomSeed(GetTime());
        g.has_been_initialized = true;
    }
    return true;
}

void Game_spawn_scenes(Game& g) {
    mPrint("Spawning scenes...");
    TitleScene_create(g.title_scene);
    TitleScene_init(g.title_scene);
    TitleScene_set_id(g.title_scene, next_scene_id++);

    //TitleScene_set_popup_manager(g.title_scene, g.popup_manager);
    g.title_scene.popup_manager = g.popup_manager;

    g.scene_keys["title"] = g.title_scene.id;

    //g.gameplay_scene = make_shared<GameplayScene>();
    //g.gameplay_scene.init();
    GameplayScene_create(g.gameplay_scene);
    GameplayScene_init(g.gameplay_scene);
    //GameplayScene_set_id(g.gameplay_scene, next_scene_id++);
    //GameplayScene_set_popup_manager(g.gameplay_scene, g.popup_manager);

    g.gameplay_scene.id = next_scene_id++;
    g.gameplay_scene.popup_manager = g.popup_manager;
    g.scene_keys["gameplay"] = g.gameplay_scene.id;
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
    switch(g.current_scene) {
    case SCENE_TITLE:
        TitleScene_handle_input(g.title_scene);
        break;
    case SCENE_GAMEPLAY:
        GameplayScene_handle_input(g.gameplay_scene);
        break;
    default:
        break;
    }
}

inline void Game_update(Game& g) {
    switch(g.current_scene) {
    case SCENE_TITLE:
        TitleScene_update(g.title_scene);
        break;
    case SCENE_GAMEPLAY:
        GameplayScene_update(g.gameplay_scene);
        break;
    default:
        break;
    }
}

inline void Game_cleanup(Game& g) {
    switch(g.current_scene) {
    case SCENE_TITLE:
        TitleScene_cleanup(g.title_scene);
        break;
    case SCENE_GAMEPLAY:
        GameplayScene_cleanup(g.gameplay_scene);
        break;
    default:
        break;
    }
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

    //mPrint("Handling transition out...");

    const Color c = (Color){0x66, 0x66, 0x66};

    float a = 0;
    const int w = GetScreenWidth();
    const int h = GetScreenHeight();
    const float transition_speed = 0.040f;

    //mPrint("switch...");

    switch(g.current_scene) {
    case SCENE_TITLE:
        a = TitleScene_get_alpha(g.title_scene);
        break;
    case SCENE_GAMEPLAY:
        //a = g.gameplay_scene->get_alpha();
        a = g.gameplay_scene.alpha;
        break;
    default:
        break;
    }

    //mPrint("drawing rectangle...");
    DrawRectangle(0, 0, w, h, Fade(c, a));

    //mPrint("if alpha is less than 1.0f...");
    if(a < 1.0f) {
        if(g.current_scene == SCENE_TITLE) {
            g.title_scene.alpha += transition_speed;
        } else if(g.current_scene == SCENE_GAMEPLAY) {
            g.gameplay_scene.alpha += transition_speed;
        }

    } else {
        if(g.current_scene == SCENE_TITLE) {
            TitleScene_close(g.title_scene);
            g.gameplay_scene.transition = SCENE_TRANSITION_IN;
            g.current_scene = SCENE_GAMEPLAY;

        } else if(g.current_scene == SCENE_GAMEPLAY) {
            GameplayScene_close(g.gameplay_scene);
            TitleScene_set_scene_transition(g.title_scene, SCENE_TRANSITION_IN);
            g.current_scene = SCENE_TITLE;
        }
    }

    //mPrint("End of Handling transition out...");
}

void Game_handle_transition_in(Game& g) {
    //mPrint("Handling transition in...");

    const Color c = (Color){0x66, 0x66, 0x66};
    float a = 0;
    switch(g.current_scene) {
    case SCENE_TITLE:
        a = TitleScene_get_alpha(g.title_scene);
        break;
    case SCENE_GAMEPLAY:
        a = g.gameplay_scene.alpha;
        break;
    default:
        break;
    }

    const int w = GetScreenWidth();
    const int h = GetScreenHeight();
    const float transition_speed = 0.040f;

    if(a > 0.0f) {
        DrawRectangle(0, 0, w, h, Fade(c, a));

        switch(g.current_scene) {
        case SCENE_TITLE:
            TitleScene_set_alpha(g.title_scene,
                                 TitleScene_get_alpha(g.title_scene) - transition_speed);
            break;
        case SCENE_GAMEPLAY:
            g.gameplay_scene.alpha -= transition_speed;
            break;
        default:
            break;
        }

    } else {
        //g.scenes[g.current_scene_id]->set_scene_transition(SCENE_TRANSITION_NONE);

        if(g.current_scene == SCENE_TITLE) {
            g.title_scene.transition = SCENE_TRANSITION_NONE;
        } else if(g.current_scene == SCENE_GAMEPLAY) {
            g.gameplay_scene.transition = SCENE_TRANSITION_NONE;
        }
    }
    //mPrint("End of Handling transition in...");
}

SceneTransition Game_get_current_scene_transition(Game& g) {
    switch(g.current_scene) {
    case SCENE_TITLE:
        return g.title_scene.transition;
    case SCENE_GAMEPLAY:
        return g.gameplay_scene.transition;
    }
    return SCENE_TRANSITION_NONE;
}

inline void Game_draw(Game& g) {
    BeginDrawing();
    BeginTextureMode(g.target);
    ClearBackground(BLACK);
    switch(g.current_scene) {
    case SCENE_TITLE:
        TitleScene_draw(g.title_scene);
        break;
    case SCENE_GAMEPLAY:
        GameplayScene_draw(g.gameplay_scene);
        break;
    }

    EndTextureMode();
    DrawTextureRec(g.target.texture, g.screen_rect, (Vector2){0, 0}, WHITE);

    switch(Game_get_current_scene_transition(g)) {
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

int Game_run(Game& g) {
    if(!g.has_been_initialized) {
        mPrint("Game has not been initialized. Exiting...");
        return EXIT_FAILURE;
    }
    while(!WindowShouldClose()) {
        Game_handle_input(g);
        Game_update(g);
        Game_draw(g);
        Game_cleanup(g);
    }
    mPrint("Window closed.");
    return EXIT_SUCCESS;
}

void Game_close(Game& g) {
    mPrint("Closing game...");
    // have to close the scene first otherwise it crashes
    // CloseWindow() must be the LAST raylib call before exiting
    //g.scenes[g.current_scene_id]->close();

    switch(g.current_scene) {
    case SCENE_TITLE:
        TitleScene_close(g.title_scene);
        break;
    case SCENE_GAMEPLAY:
        GameplayScene_close(g.gameplay_scene);
        break;
    default:
        break;
    }

    mPrint("Unloading fonts...");
    UnloadFont(g.global_font);

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
