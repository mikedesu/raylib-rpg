#pragma once

#include "Scene.h"

using namespace std;

class GameoverScene : public Scene {
public:
  GameoverScene() {
    mPrint("GameoverScene constructor");
    set_control_mode(CONTROL_MODE_PLAYER);
    set_texture_filepath("gameover_textures.txt");
    set_global_scale(16.0f);
    set_scene_transition(SCENE_TRANSITION_IN);
    set_scene_type(SCENE_TYPE_GAMEOVER);
  }

  void handle_input() {
    if (IsKeyPressed(KEY_D)) {
      flip_debug_panel();
    } else if (IsKeyPressed(KEY_SPACE)) {
      set_scene_transition(SCENE_TRANSITION_OUT);
    }
  }

  bool init() {
    if (!get_has_been_initialized()) {
      set_camera_default_values();
      load_fonts();
      bool result = load_textures();
      if (!result) {
        mPrint("Error loading textures. Exiting...");
        return false;
      }
      int w = get_textures()["gameover"]->texture.width;
      int h = get_textures()["gameover"]->texture.height;
      int x = GetScreenWidth() / 2.0 - w * get_global_scale() / 2;
      int y = GetScreenHeight() / 4;
      float scale = 8.0f;
      spawn_entity("gameover", x, y, SPRITETYPE_PLAYER, false);
      y += h * get_global_scale();
      w = get_textures()["press-space-to-return"]->texture.width;
      x = GetScreenWidth() / 2.0 - w * scale / 2;
      spawn_entity("press-space-to-return", x, y, SPRITETYPE_PLAYER, false,
                   scale);
      set_has_been_initialized(true);
    }
    return true;
  }

  void draw_debug_panel() {
    string camera_info_str =
        "Current Frame: " + to_string(get_current_frame()) + "\n" +
        "Control mode: " + to_string(get_control_mode()) + "\n" +
        "Camera target: " + to_string(get_camera2d().target.x) + ", " +
        to_string(get_camera2d().target.y) + "\n" + "GameoverScene";
    DrawRectangle(0, 0, 500, 200, Fade(BLACK, 0.5f));
    DrawTextEx(get_global_font(), camera_info_str.c_str(), (Vector2){10, 10},
               16, 0.5f, WHITE);
  }

  void draw() {
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

  ~GameoverScene() {}
  void update() {}
  void cleanup() {}
};
