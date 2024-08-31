#pragma once

#include <raylib.h>
#include <string>

using std::string;

typedef struct {

    RenderTexture target;
    float alpha = 1.0f;
    int fontsize = 24;

} PopupManager;

void PopupManager_create(PopupManager& p);
void PopupManager_destroy(PopupManager& p);
void PopupManager_draw(PopupManager& p, const float x, const float y);
void PopupManager_render(PopupManager& p, string s);

//void PopupManager_zero_alpha() { alpha = 0; }
//void PopupManager_set_fontsize(const int size) { fontsize = size; }
//const int PopupManager_get_fontsize() const { return fontsize; }
