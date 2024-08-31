#pragma once

#include "raylib.h"
#include <string>

using std::string;

typedef struct {
    Texture2D texture;
    int num_frames;
    int contexts;
    int is_player;
    string asset_path;
    int width;
    int height;
} TextureInfo;
