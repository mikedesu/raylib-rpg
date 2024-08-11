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

using std::make_shared;
using std::shared_ptr;
using std::string;
using std::unordered_map;
using std::vector;

class GameoverScene : public Scene {
public:
  GameoverScene();
  ~GameoverScene();

  void draw();
  void update();
  bool init();
  void handle_input();
  void draw_debug_panel();
  void cleanup();

private:
  // Music music;
};
