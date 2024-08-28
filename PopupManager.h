#pragma once

#include <raylib.h>
#include <string>

using std::string;

class PopupManager {
private:
  RenderTexture target;
  float alpha = 1.0f;
  int fontsize = 24;

public:
  PopupManager() { target = LoadRenderTexture(600, 400); }

  ~PopupManager() {}

  void draw(const float x, const float y) {
    DrawTextureRec(
        target.texture,
        {0, 0, (float)target.texture.width, (float)-target.texture.height},
        {x, y}, Fade(WHITE, alpha));
    // DrawRectangleLines(x, y, target.texture.width, target.texture.height,
    // WHITE);
    alpha -= 0.01f;
  }

  void render(string s) {
    BeginDrawing();
    BeginTextureMode(target);
    ClearBackground(BLANK);
    DrawText(s.c_str(), 10, 10, fontsize, WHITE);
    EndTextureMode();
    EndDrawing();
    alpha = 1.0f;
  }

  void zero_alpha() { alpha = 0; }
  void set_fontsize(const int size) { fontsize = size; }
  const int get_fontsize() const { return fontsize; }
};
