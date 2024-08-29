#pragma once

#include "Scene.h"
#include "Sprite.h"
#include "control_mode.h"
#include "entity_id.h"
#include "raylib.h"
#include "texture_info.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// using std::make_shared;
// using std::shared_ptr;
// using std::string;
// using std::unordered_map;
// using std::vector;

using namespace std;

class TitleScene : public Scene {
public:
  TitleScene();
  ~TitleScene();

  void draw();
  void update();
  bool init();
  void handle_input();
  void draw_debug_panel();
  void cleanup();
  void close();

  void prerender_texture();

private:
  // Music music;
  RenderTexture prerendered_texture;
  Rectangle src_rect;
  Rectangle dst_rect;
  Vector2 origin = (Vector2){0, 0};
};
