#include "TitleScene.h"
#include "mPrint.h"

#include <raylib.h>

TitleScene::TitleScene() {
  mPrint("TitleScene constructor");
  set_control_mode(CONTROL_MODE_PLAYER);
  set_texture_filepath("title_textures.txt");
  set_scene_transition(SCENE_TRANSITION_IN);
  set_scene_type(SCENE_TYPE_TITLE);
}

TitleScene::~TitleScene() { mPrint("TitleScene destructor"); }

void TitleScene::update() {
  // UpdateMusicStream(get_music());
}

void TitleScene::handle_input() {
  if (IsKeyPressed(KEY_D)) {
    flip_debug_panel();
  } else if (IsKeyPressed(KEY_SPACE)) {
    set_scene_transition(SCENE_TRANSITION_OUT);
  } else if (IsKeyPressed(KEY_F)) {
    ToggleFullscreen();
  }
}

bool TitleScene::init() {
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
    // if (IsMusicReady(get_music())) {
    //   mPrint("### MUSIC IS READY ###");
    //   SetMusicVolume(get_music(), 1.0f);
    //   PlayMusicStream(get_music());
    //  UpdateMusicStream(get_music());
    //} else {
    //  mPrint("### MUSIC NOT READY ###");
    //}
    // play_music();
    // Mix_PlayMusic(get_music(), -1);
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

void TitleScene::prerender_texture() {
  BeginDrawing();
  BeginTextureMode(prerendered_texture);
  BeginMode2D(get_camera2d());
  Color clear_color = BLACK;
  ClearBackground(clear_color);
  shared_ptr<texture_info> title_info = get_texture_info("title");
  shared_ptr<texture_info> presents_info = get_texture_info("presents");
  if (title_info == nullptr) {
    mPrint("Error: title_info is null.");
    return;
  }
  if (presents_info == nullptr) {
    mPrint("Error: presents_info is null.");
    return;
  }
  int scale = 1;
  float w_src = title_info->texture.width;
  float h_src = title_info->texture.height;
  float w_dst = w_src;
  float h_dst = h_src;
  float x = 0;
  float y = 0;
  Vector2 origin = (Vector2){0, 0};
  Rectangle src_rect = {0, 0, w_src, h_src};
  Rectangle dst_rect = {0, 0, w_dst, h_dst};
  while (w_dst < GetScreenWidth() && h_dst < GetScreenHeight()) {
    scale *= 2;
    w_dst *= 2;
    h_dst *= 2;
  }
  w_dst /= 2;
  h_dst /= 2;
  scale /= 2;
  x = GetScreenWidth() / 2.0f - w_dst / 2.0f;
  y = GetScreenHeight() / 2.0f - h_dst;
  dst_rect.x = x;
  dst_rect.y = y;
  dst_rect.width = w_dst;
  dst_rect.height = h_dst;
  DrawTexturePro(title_info->texture, src_rect, dst_rect, origin, 0, WHITE);
  w_src = presents_info->texture.width;
  h_src = presents_info->texture.height;
  src_rect = {0, 0, w_src, h_src};
  w_dst = w_src * scale / 4;
  h_dst = h_src * scale / 4;
  y -= h_dst;
  dst_rect.y = y;
  dst_rect.width = w_dst;
  dst_rect.height = h_dst;
  DrawTexturePro(presents_info->texture, src_rect, dst_rect, origin, 0, WHITE);
  EndMode2D();
  EndTextureMode();
  EndDrawing();
}

void TitleScene::draw() {
  BeginMode2D(get_camera2d());
  Color clear_color = BLACK;
  ClearBackground(clear_color);
  DrawTexturePro(prerendered_texture.texture, src_rect, dst_rect, origin, 0.0f,
                 WHITE);
  EndMode2D();
  // draw debug panel
  if (get_debug_panel_on()) {
    DrawFPS(GetScreenWidth() - 80, 10);
    draw_debug_panel();
  }
  incr_current_frame();
}

void TitleScene::draw_debug_panel() {
  string camera_info_str =
      "Current Frame: " + to_string(get_current_frame()) + "\n" +
      "Control mode: " + to_string(get_control_mode()) + "\n" +
      "Camera target: " + to_string(get_camera2d().target.x) + ", " +
      to_string(get_camera2d().target.y) + "\n" + "TitleScene";
  const int fontsize = get_global_font().baseSize;
  DrawTextEx(get_global_font(), camera_info_str.c_str(), (Vector2){10, 10},
             fontsize, 0.5f, WHITE);
}

void TitleScene::cleanup() {
  // mPrint("Cleaning up TitleScene...");
}

void TitleScene::close() {
  Scene::close();
  UnloadRenderTexture(prerendered_texture);
}
