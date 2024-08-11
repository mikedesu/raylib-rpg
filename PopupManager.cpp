#include "PopupManager.h"
// #include <iostream>

// using std::cout;
// using std::endl;

PopupManager::PopupManager() {
  // Constructor
  target = LoadRenderTexture(600, 400);
}

PopupManager::~PopupManager() {
  // Destructor
  // UnloadRenderTexture(target);
}

void PopupManager::zero_alpha() { alpha = 0; }

void PopupManager::draw(const float x, const float y) {
  DrawTextureRec(
      target.texture,
      {0, 0, (float)target.texture.width, (float)-target.texture.height},
      {x, y}, Fade(WHITE, alpha));
  // draw a outline around the popup
  // DrawRectangleLines(x, y, target.texture.width, target.texture.height,
  // WHITE);

  alpha -= 0.01f;
}

void PopupManager::render(string s) {
  BeginDrawing();
  BeginTextureMode(target);
  ClearBackground(BLANK);
  // DrawRectangle(0, 0, 200, 200, RED);
  DrawText(s.c_str(), 10, 10, 24, WHITE);
  EndTextureMode();
  EndDrawing();
  alpha = 1.0f;
}
