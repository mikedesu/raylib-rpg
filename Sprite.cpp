#include "Sprite.h"

Sprite::Sprite(Texture2D &t, const unsigned int frames, const unsigned int c,
               const float x, const float y, const int w, const int h,
               sprite_type spritetype) {
  texture = t;
  anim_frames = frames;
  contexts = c;
  width = w;
  height = h;
  set_scale(1.0f);
  init_rects(w, h);
  origin = (Vector2){0, 0};
  dest.x = x;
  dest.y = y;
  current_frame = 0;
  velocity = (Vector2){0, 0};
  is_marked_for_deletion = false;
  is_animating = false;
  is_flipped = false;
  type = spritetype;
  is_spinning = false;
  velocity.x = 0;
  velocity.y = 0;
  acceleration.x = 0;
  acceleration.y = 0;
}
