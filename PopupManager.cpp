#include "PopupManager.h"

void PopupManager_create(PopupManager& p) {
    p.target = LoadRenderTexture(600, 400);
}

void PopupManager_destroy(PopupManager& p) { }

void PopupManager_draw(PopupManager& p, const float x, const float y) {
    DrawTextureRec(p.target.texture,
                   {0, 0, (float)p.target.texture.width, (float)-p.target.texture.height},
                   {x, y},
                   Fade(WHITE, p.alpha));
    // DrawRectangleLines(x, y, target.texture.width, target.texture.height,
    // WHITE);
    p.alpha -= 0.01f;
}

void PopupManager_render(PopupManager& p, string s) {
    BeginDrawing();
    BeginTextureMode(p.target);
    ClearBackground(BLANK);
    DrawText(s.c_str(), 10, 10, p.fontsize, WHITE);
    EndTextureMode();
    EndDrawing();
    p.alpha = 1.0f;
}
