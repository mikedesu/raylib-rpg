#pragma once

#include "DungeonEvent.h"
#include "raylib.h"
#include <vector>

using std::vector;

class MessagePane {

public:
  MessagePane(Font &f, const int fontsize, const int max_messages, const int w,
              const int h, vector<DungeonEvent> &dungeon_events)
      : font(f), font_size(fontsize), max_messages(max_messages), w(w), h(h),
        dungeon_events(dungeon_events) {}
  void draw();

private:
  Font &font;
  const int font_size;
  const int max_messages;
  const int w;
  const int h;
  vector<DungeonEvent> &dungeon_events;
};
