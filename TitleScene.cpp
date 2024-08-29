#include "TitleScene.h"
#include "mPrint.h"

void TitleScene::prerender_texture() {
  int scale = 1;
  float x = 0, y = 0;
  Vector2 origin = (Vector2){0, 0};
  Color clear_color = BLACK;
  shared_ptr<texture_info> title_info = get_texture_info("title");
  shared_ptr<texture_info> presents_info = get_texture_info("presents");
  float w_src = title_info->texture.width, h_src = title_info->texture.height;
  float w_dst = w_src, h_dst = h_src;
  Rectangle src_rect = {0, 0, w_src, h_src}, dst_rect = {0, 0, w_dst, h_dst};
  BeginDrawing();
  BeginTextureMode(prerendered_texture);
  BeginMode2D(get_camera2d());
  ClearBackground(clear_color);
  if (title_info == nullptr) {
    mPrint("Error: title_info is null.");
    return;
  } else if (presents_info == nullptr) {
    mPrint("Error: presents_info is null.");
    return;
  }
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
  dst_rect.y = y - h_src * scale / 4;
  dst_rect.width = w_src * scale / 4;
  dst_rect.height = h_src * scale / 4;
  DrawTexturePro(presents_info->texture, src_rect, dst_rect, origin, 0, WHITE);
  EndMode2D();
  EndTextureMode();
  EndDrawing();
}
