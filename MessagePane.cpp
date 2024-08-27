#include "MessagePane.h"

void MessagePane::draw() {
  int count = 0;
  string s = "Messages:\n\n";
  DrawRectangle(x, y, w, h, bgcolor);
  for (int i = dungeon_events.size() - 1; i >= 0 && count < max_messages; i--) {
    s += dungeon_events[i].get_message() + "\n";
    count++;
  }
  DrawRectangleLines(x, y, w, h, bordercolor);
  DrawTextEx(font, s.c_str(), (Vector2){x + pad, y + pad}, font_size, 0.5f,
             fgcolor);
}
