#include "GameplayScene.h"
// #include "Sprite.h"
#include "mPrint.h"

GameplayScene::GameplayScene() {
  mPrint("GameplayScene constructor");
  set_control_mode(CONTROL_MODE_PLAYER);
  set_texture_filepath("game_textures.txt");
  set_global_scale(4.0f);
  // set_global_scale(4.0f);
  set_scene_transition(SCENE_TRANSITION_IN);
  set_scene_type(SCENE_TYPE_GAMEPLAY);
  // load_music("/home/darkmage/Music/darkmage/lets-fkn-go.mp3");
}

GameplayScene::~GameplayScene() { mPrint("GameplayScene destructor"); }

void GameplayScene::gameover() { set_scene_transition(SCENE_TRANSITION_OUT); }

void GameplayScene::update_player_movement() {
  /*
   */
}

void GameplayScene::update_enemy_movement() {
  /*
   */
}

void GameplayScene::handle_player_collision() {
  /*
   */
}

void GameplayScene::update() {
  /*
   */
}

void GameplayScene::handle_input() {
  /*
   */
  if (IsKeyPressed(KEY_D)) {
    flip_debug_panel();
  }

  if (IsKeyPressed(KEY_F)) {
    ToggleFullscreen();
  }
}

bool GameplayScene::init() {
  if (!get_has_been_initialized()) {
    mPrint("Initializing gameplay scene...");
    mPrint("Initializing camera...");
    set_camera_default_values();
    mPrint("Loading assets...");
    load_fonts();
    bool result = load_textures();
    if (!result) {
      mPrint("Error loading textures. Exiting...");
      return false;
    }
    mPrint("Spawning player...");

    mPrint("Loading sound effects...");

    set_has_been_initialized(true);

    mPrint("GameplayScene initialized");
  }
  return true;
}

void GameplayScene::draw_debug_panel() {
  string camera_info_str =
      "Current Frame: " + to_string(get_current_frame()) + "\n" +
      "Control mode: " + to_string(get_control_mode()) + "\n" +
      "Player Position: " + to_string(get_sprites()[player_id]->get_x()) +
      ", " + to_string(get_sprites()[player_id]->get_y()) + "\n" +
      "Player Velocity: " + to_string(get_sprites()[player_id]->get_vx()) +
      ", " + to_string(get_sprites()[player_id]->get_vy()) + "\n" +
      "Player Acceleration: " + to_string(get_sprites()[player_id]->get_ax()) +
      ", " + to_string(get_sprites()[player_id]->get_ay()) + "\n" +
      "Player HP: " + to_string(get_sprites()[player_id]->get_hp()) + "/" +
      to_string(get_sprites()[player_id]->get_maxhp()) + "\n" +
      "Camera target: " + to_string(get_camera2d().target.x) + ", " +
      to_string(get_camera2d().target.y) + "\n" + "GameplayScene" +
      "Sprites: " + to_string(get_sprites().size()) + "\n" +
      "IsPaused: " + to_string(get_paused()) + "\n";
  DrawRectangle(0, 0, 500, 200, Fade(BLACK, 0.5f));
  DrawTextEx(get_global_font(), camera_info_str.c_str(), (Vector2){10, 10}, 16,
             0.5f, WHITE);
}

void GameplayScene::draw_hud() {
  /*
   */
}

void GameplayScene::cleanup() {
  // mPrint("cleanup");
  for (int i = 0; i < (int)get_entity_ids().size(); i++) {
    entity_id id = get_entity_ids()[i];
    if (get_sprites()[id]->get_is_marked_for_deletion()) {
      get_sprites().erase(id);
      // also need to erase from entity_ids
      get_entity_ids().erase(get_entity_ids().begin() + i);
      // update the index so that we don't skip any
      i--;
    }
  }
}

void GameplayScene::draw() {
  BeginMode2D(get_camera2d());
  Color clear_color = (Color){0x10, 0x10, 0x10, 0xFF};
  ClearBackground(clear_color);

  for (auto &s : get_sprites()) {
    s.second->draw();
    if (get_debug_panel_on()) {
      s.second->draw_hitbox();

      // draw a line from the sprite to the player
      // if (s.second->get_type() == SPRITETYPE_ENEMY) {
      //  DrawLine(s.second->get_x(), s.second->get_y(),
      //           get_sprites()[player_id]->get_x(),
      //           get_sprites()[player_id]->get_y(), RED);
      //}
    }
  }

  EndMode2D();

  handle_draw_debug_panel();

  if (show_test_popup) {
    if (get_popup_manager() != nullptr) {

      const float x = GetScreenWidth() / 2.0f - 100.0f;
      const float y = GetScreenHeight() / 2.0f - 100.0f;
      //  get player x
      // const float x = get_sprite(player_id)->get_x();
      // get player y
      // const float y = get_sprite(player_id)->get_y();
      Vector2 s = GetWorldToScreen2D(
          (Vector2){x - 50, y - 50},
          get_camera2d()); // Get the screen space position for
                           // a 2d camera world space position
      get_popup_manager()->draw(s.x, s.y);
    }
  }

  incr_current_frame();
}

inline void GameplayScene::handle_draw_debug_panel() {
  if (get_debug_panel_on()) {
    DrawFPS(GetScreenWidth() - 80, 10);
    draw_debug_panel();
  } else if (get_hud_on()) {
    draw_hud();
  }
}

void GameplayScene::close() {
  mPrint("Closing gameplay scene...");
  mPrint("Unloading textures...");
  for (auto &t : get_textures()) {
    UnloadTexture(t.second.texture);
  }
  mPrint("Clearing textures...");
  get_textures().clear();
  mPrint("Clearing sprites...");
  get_sprites().clear();
  get_bgsprites().clear();
  mPrint("Clearing entity ids...");
  get_entity_ids().clear();
  get_bg_entity_ids().clear();
  mPrint("Unloading font...");
  UnloadFont(get_global_font());
  // mPrint("Clearing stars...");
  // get_stars().clear();

  // if (music != NULL) {
  //  stop music
  //  Mix_PauseMusic();
  //  Mix_FreeMusic(music);
  //}

  set_has_been_initialized(false);
  player_id = -1;
  get_popup_manager()->zero_alpha();
  mPrint("Scene closed.");
}
