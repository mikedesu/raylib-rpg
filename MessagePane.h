#pragma once

#include "DungeonEvent.h"
#include "raylib.h"
#include <vector>

using std::vector;

class MessagePane {

public:
  MessagePane(Font &f, const int fontsize, vector<DungeonEvent> &dungeon_events)
      : font(f), font_size(fontsize), dungeon_events(dungeon_events) {}
  void draw();

private:
  vector<DungeonEvent> &dungeon_events;
  Font &font;
  int font_size;
};
