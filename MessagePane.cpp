#include "MessagePane.h"

void MessagePane::draw() {
  int count = 0;
  const int pad = 10;
  const float x = GetScreenWidth() - w - 10;
  const float y = h + 3 * pad;
  const Color c0 = Fade(BLACK, 0.5f);
  string s = "Messages:\n\n";
  DrawRectangle(x, y, w, h, c0);
  for (int i = dungeon_events.size() - 1; i >= 0 && count < max_messages; i--) {
    s += dungeon_events[i].get_message() + "\n";
    count++;
  }
  DrawRectangleLines(x, y, w, h, GRAY);
  DrawTextEx(font, s.c_str(), (Vector2){x + pad, y + pad}, font_size, 0.5f,
             WHITE);
}
