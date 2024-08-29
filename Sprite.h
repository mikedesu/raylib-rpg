#pragma once

#include "mPrint.h"
#include "movement_type.h"
#include "raylib.h"

typedef enum {
  SPRITETYPE_NONE,
  SPRITETYPE_TITLE,
  SPRITETYPE_PLAYER,
  SPRITETYPE_ENEMY,
  SPRITETYPE_ITEM,
  SPRITETYPE_WALL,
  SPRITETYPE_COUNT
} sprite_type;

class Sprite {
private:
  Texture2D texture;
  Rectangle src;
  Rectangle flipped_src;
  Rectangle dest;
  Rectangle hitbox = {0, 0, 0, 0};
  unsigned int alpha;
  int width = 0;
  int height = 0;
  int context = 0;
  int contexts = 0;
  unsigned int anim_frames;
  unsigned int current_frame;
  unsigned int frame_counter;
  float scale = 1.0f;
  float rotation_angle = 0.0f;
  float rotation_speed = 1.0f;
  Vector2 origin;
  Vector2 velocity;
  Vector2 acceleration;
  bool is_marked_for_deletion = false;
  bool is_animating = false;
  bool is_flipped = false;
  bool is_spinning = false;
  sprite_type type = SPRITETYPE_NONE;
  movement_type movement = MOVEMENT_TYPE_NONE;

public:
  Sprite(Texture2D &texture, const unsigned int anim_frames,
         const unsigned int contexts, const float x, const float y, const int w,
         const int h, sprite_type t);

  ~Sprite() {}

  void draw() {
    const Color color = WHITE;
    const float angle = 0.0f;
    //  rlPushMatrix();
    //  rlTranslatef(dest.x + dest.width / 2, dest.y + dest.height / 2, 0);
    //  rlRotatef(rotation_angle, 0, 0, 1);
    //   rlTranslatef(0, 0, 0);
    //  rlTranslatef(-dest.x - dest.width / 2, -dest.y - dest.height / 2, 0);
    if (is_flipped) {
      DrawTexturePro(texture, flipped_src, dest, origin, angle, color);
    } else {
      DrawTexturePro(texture, src, dest, origin, angle, color);
    }
    // rlPopMatrix();
    const int frame_freq = 10;
    if (is_animating && frame_counter % frame_freq == 0) {
      incr_frame();
    }
    frame_counter++;
  }

  void draw_hitbox() {
    DrawRectangleLines(hitbox.x, hitbox.y, hitbox.width, hitbox.height, RED);
  }
  void move(const float x, const float y) {
    dest.x += x * scale;
    dest.y += y * scale;
  }
  void move_rect(Rectangle &r) {
    dest.x = r.x;
    dest.y = r.y;
  }
  void mark_for_deletion() { is_marked_for_deletion = true; }
  void incr_frame() {
    current_frame++;
    if (current_frame >= anim_frames) {
      current_frame = 0;
    }
    src.x = (float)current_frame * src.width;
    flipped_src.x = src.x;
  }
  void incr_ax(const float ax) { acceleration.x += ax; }
  void incr_ay(const float ay) { acceleration.y += ay; }
  void incr_vx(const float vx) { velocity.x += vx; }
  void incr_vy(const float vy) { velocity.y += vy; }
  void set_scale(const float s) {
    scale = s;
    dest.width = (float)width * scale;
    dest.height = (float)height * scale;
    // init_rects(width, height);
  }
  void set_current_frame(const unsigned int frame) {
    if (frame >= anim_frames) {
      return;
    }
    current_frame = frame;
    // update the src rectangle
    src.x = (float)frame * src.width;
    src.y = 0;
    flipped_src.x = src.x;
  }
  void set_x(const float x) { dest.x = x; }
  void set_y(const float y) { dest.y = y; }
  void set_vx(const float vx) { velocity.x = vx; }
  void set_vy(const float vy) { velocity.y = vy; }
  void set_velocity(const Vector2 v) { velocity = v; }
  void set_acceleration(const Vector2 a) { acceleration = a; }
  void set_is_animating(const bool is_animating) {
    this->is_animating = is_animating;
  }
  void set_is_flipped(const bool is_flipped) { this->is_flipped = is_flipped; }
  void set_type(const sprite_type t) { type = t; }
  void set_ax(const float ax) { acceleration.x = ax; }
  void set_ay(const float ay) { acceleration.y = ay; }
  void set_context(const int n) {
    if (n >= contexts) {
      mPrint("Error: context out of bounds.");
      context = 0;
    } else {
      context = n;
    }
    // whenever the context is updated, we have to re-initialize our rects
    init_rects(width, height);
  }
  void flip() { is_flipped = !is_flipped; }

  void update(const Vector2 dungeon_position) {
    const int tilesize = 20;
    set_x(dungeon_position.x * tilesize * scale);
    set_y(dungeon_position.y * tilesize * scale);
    if (is_spinning) {
      rotation_angle += rotation_speed;
    }
  }

  void set_rotation_angle(const float angle) { rotation_angle = angle; }

  void set_is_spinning(const bool is_spinning) {
    this->is_spinning = is_spinning;
  }
  void set_rotation_speed(const float speed) { rotation_speed = speed; }
  void set_movement_type(const movement_type m) { movement = m; }
  void set_alpha(const unsigned int alpha) { this->alpha = alpha; }

  const Rectangle get_dest() const { return dest; }
  const Rectangle get_hitbox() const { return hitbox; }
  const unsigned int get_alpha() const { return alpha; }
  const int get_width() const { return width; }
  const int get_height() const { return height; }
  const int get_anim_frames() const { return anim_frames; }
  const int get_current_frame() const { return current_frame; }
  const int get_context() const { return context; }
  const int get_contexts() const { return contexts; }
  const float get_vx() const { return velocity.x; }
  const float get_vy() const { return velocity.y; }
  const float get_ax() const { return acceleration.x; }
  const float get_ay() const { return acceleration.y; }
  const float get_x() const { return dest.x; }
  const float get_y() const { return dest.y; }
  const float get_scale() const { return scale; }
  const float get_rotation_angle() const { return rotation_angle; }
  const float get_rotation_speed() const { return rotation_speed; }
  const Vector2 get_velocity() const { return velocity; }
  const Vector2 get_acceleration() const { return acceleration; }
  const bool get_is_marked_for_deletion() const {
    return is_marked_for_deletion;
  }
  const bool get_is_animating() const { return is_animating; }
  const bool get_is_flipped() const { return is_flipped; }
  const sprite_type get_type() const { return type; }
  const bool get_is_spinning() const { return is_spinning; }

  void init_rects() {
    src = (Rectangle){0, 0, (float)texture.width / anim_frames,
                      (float)texture.height};
    flipped_src = (Rectangle){src.x, src.y, src.width * -1.0f, src.height};
    dest = (Rectangle){dest.x, dest.y,
                       ((float)texture.width / anim_frames) * scale,
                       (float)texture.height * scale};
    hitbox = (Rectangle){dest.x, dest.y, dest.width, dest.height};
  }

  void init_rects(const float w, const float h) {

    src = (Rectangle){0, h * context, w, h};
    flipped_src = (Rectangle){src.x, src.y, w * -1.0f, h};
    dest = (Rectangle){dest.x, dest.y, w * scale, h * scale};
    hitbox = (Rectangle){dest.x, dest.y, dest.width, dest.height};
  }
};
