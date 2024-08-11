#include "GameoverScene.h"

#include "mPrint.h"
// #include <cassert>
// #include <cstring>

// static entity_id next_entity_id = 0;

GameoverScene::GameoverScene() {
  mPrint("GameoverScene constructor");
  set_control_mode(CONTROL_MODE_PLAYER);
  set_texture_filepath("gameover_textures.txt");
  set_global_scale(16.0f);
  set_scene_transition(SCENE_TRANSITION_IN);
  set_scene_type(SCENE_TYPE_GAMEOVER);

  // load_music("audio/skull-title.wav");
  //  load_music("audio/skull-title-0.mp3");
  //   load_music("/home/darkmage/Music/darkmage/lets-fkn-go.mp3");
  //    music =
  //    LoadMusicStream("/home/darkmage/Music/darkmage/lets-fkn-go.mp3");
}

GameoverScene::~GameoverScene() { mPrint("GameoverScene destructor"); }

void GameoverScene::update() {}

void GameoverScene::handle_input() {
  if (IsKeyPressed(KEY_D)) {
    flip_debug_panel();
  } else if (IsKeyPressed(KEY_SPACE)) {
    set_scene_transition(SCENE_TRANSITION_OUT);
  } else if (IsKeyPressed(KEY_F)) {

    ToggleFullscreen();
  }
}

bool GameoverScene::init() {
  if (!get_has_been_initialized()) {
    mPrint("Initializing scene...");
    mPrint("Initializing camera...");
    set_camera_default_values();
    mPrint("Loading assets...");
    load_fonts();
    bool result = load_textures();
    if (!result) {
      mPrint("Error loading textures. Exiting...");
      return false;
    }
    int w = get_textures()["gameover"].texture.width;
    int h = get_textures()["gameover"].texture.height;
    int x = GetScreenWidth() / 2.0 - w * get_global_scale() / 2;
    int y = GetScreenHeight() / 4;
    float scale = 8.0f;
    spawn_entity("gameover", x, y, SPRITETYPE_PLAYER, false);
    y += h * get_global_scale();
    w = get_textures()["press-space-to-return"].texture.width;
    x = GetScreenWidth() / 2.0 - w * scale / 2;
    spawn_entity("press-space-to-return", x, y, SPRITETYPE_PLAYER, false,
                 scale);
    set_has_been_initialized(true);
  }
  return true;
}

void GameoverScene::draw_debug_panel() {
  string camera_info_str =
      "Current Frame: " + to_string(get_current_frame()) + "\n" +
      "Control mode: " + to_string(get_control_mode()) + "\n" +
      "Camera target: " + to_string(get_camera2d().target.x) + ", " +
      to_string(get_camera2d().target.y) + "\n" + "GameoverScene";
  DrawRectangle(0, 0, 500, 200, Fade(BLACK, 0.5f));
  DrawTextEx(get_global_font(), camera_info_str.c_str(), (Vector2){10, 10}, 16,
             0.5f, WHITE);
}

void GameoverScene::cleanup() {
  // mPrint("Cleaning up GameoverScene...");
  //  for (auto &sprite : get_sprites()) {
  //  }
  //  get_sprites().clear();
  //  get_textures().clear();
  //  set_has_been_initialized(false);
}

void GameoverScene::draw() {
  BeginMode2D(get_camera2d());

  for (auto &s : get_sprites()) {
    s.second->draw();
    if (get_debug_panel_on()) {
      s.second->draw_hitbox();
    }
  }

  if (get_debug_panel_on()) {
    DrawFPS(GetScreenWidth() - 80, 10);
    draw_debug_panel();
  } else if (get_hud_on()) {
    draw_hud();
  }

  EndMode2D();

  incr_current_frame();
}
