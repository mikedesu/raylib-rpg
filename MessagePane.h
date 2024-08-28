#pragma once

#include "DungeonEvent.h"
#include "DungeonFloor.h"

#include "raylib.h"
#include <vector>

using std::to_string;
using std::vector;

class MessagePane {

private:
  Font &font;
  const int font_size;
  const int max_messages;
  const int x;
  const int y;
  const int w;
  const int h;
  vector<DungeonEvent> &dungeon_events;
  DungeonFloor &dungeon_floor;
  const Color bgcolor = Fade(BLACK, 0.5f);
  const Color fgcolor = WHITE;
  const Color bordercolor = GRAY;
  const float pad = 10;

public:
  MessagePane(Font &f, const int fontsize, const int max_messages, const int x,
              const int y, const int w, const int h,
              vector<DungeonEvent> &dungeon_events, DungeonFloor &dungeon_floor)
      : font(f), font_size(fontsize), max_messages(max_messages), x(x), y(y),
        w(w), h(h), dungeon_events(dungeon_events),
        dungeon_floor(dungeon_floor) {}

  void draw() {
    int count = 0;
    string s = "Messages:\n\n";
    DrawRectangle(x, y, w, h, bgcolor);
    for (int i = dungeon_events.size() - 1; i >= 0 && count < max_messages;
         i--) {
      string name =
          dungeon_floor.get_entity_name(dungeon_events[i].get_actor_id());

      string position = "";
      Vector2 target = dungeon_events[i].get_target();
      int x = (int)target.x;
      int y = (int)target.y;
      position = to_string(x) + "," + to_string(y);

      s += name + " " + dungeon_events[i].get_message() + " " + position + "\n";
      count++;
    }
    DrawRectangleLines(x, y, w, h, bordercolor);
    DrawTextEx(font, s.c_str(), (Vector2){x + pad, y + pad}, font_size, 0.5f,
               fgcolor);
  }
};
