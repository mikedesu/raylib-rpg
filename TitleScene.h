#pragma once

#include "Scene.h"

using namespace std;

class TitleScene : public Scene {

private:
  // Music music;
  RenderTexture prerendered_texture;
  Rectangle src_rect;
  Rectangle dst_rect;
  Vector2 origin = (Vector2){0, 0};

public:
  TitleScene() {
    set_control_mode(CONTROL_MODE_PLAYER);
    set_texture_filepath("title_textures.txt");
    set_scene_transition(SCENE_TRANSITION_IN);
    set_scene_type(SCENE_TYPE_TITLE);
  }
  ~TitleScene() {}

  bool init() {
    if (!get_has_been_initialized()) {
      set_camera_default_values();
      load_fonts();
      bool result = load_textures();
      if (!result) {
        mPrint("Error loading textures. Exiting...");
        return false;
      }
      prerendered_texture =
          LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
      src_rect = {0, 0, (float)prerendered_texture.texture.width,
                  (float)-prerendered_texture.texture.height};
      dst_rect = {0, 0, (float)prerendered_texture.texture.width,
                  (float)prerendered_texture.texture.height};
      prerender_texture();
      set_has_been_initialized(true);
    }
    return true;
  }

  void draw() {
    BeginMode2D(get_camera2d());
    Color clear_color = BLACK;
    ClearBackground(clear_color);
    DrawTexturePro(prerendered_texture.texture, src_rect, dst_rect, origin,
                   0.0f, WHITE);
    EndMode2D();
    if (get_debug_panel_on()) {
      DrawFPS(GetScreenWidth() - 80, 10);
      draw_debug_panel();
    }
    incr_current_frame();
  }

  void handle_input() {
    if (IsKeyPressed(KEY_D)) {
      flip_debug_panel();
    } else if (IsKeyPressed(KEY_SPACE)) {
      set_scene_transition(SCENE_TRANSITION_OUT);
    } else if (IsKeyPressed(KEY_F)) {
      ToggleFullscreen();
    }
  }

  void draw_debug_panel() {
    string camera_info_str =
        "Current Frame: " + to_string(get_current_frame()) + "\n" +
        "Control mode: " + to_string(get_control_mode()) + "\n" +
        "Camera target: " + to_string(get_camera2d().target.x) + ", " +
        to_string(get_camera2d().target.y) + "\n" + "TitleScene";
    const int fontsize = get_global_font().baseSize;
    DrawTextEx(get_global_font(), camera_info_str.c_str(), (Vector2){10, 10},
               fontsize, 0.5f, WHITE);
  }

  void close() {
    Scene::close();
    UnloadRenderTexture(prerendered_texture);
  }

  void cleanup() {}
  void update() {}

  void prerender_texture();
};
