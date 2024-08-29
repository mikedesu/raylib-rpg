#pragma once

#include "movement_type.h"
#include "raylib.h"

typedef enum {
  SPRITETYPE_NONE,
  SPRITETYPE_TITLE,
  SPRITETYPE_PLAYER,
  SPRITETYPE_ENEMY,
  SPRITETYPE_ITEM,
  SPRITETYPE_WALL,
  // SPRITETYPE_SOULSHARD,
  // SPRITETYPE_POWERUP_HEART,
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

  void draw();
  void draw_hitbox();
  void move(const float x, const float y);
  void move_rect(Rectangle &r);
  void mark_for_deletion();
  void incr_frame();
  void incr_ax(const float ax);
  void incr_ay(const float ay);
  void incr_vx(const float vx);
  void incr_vy(const float vy);
  void set_scale(const float scale);
  void set_current_frame(const unsigned int frame);
  void set_x(const float x);
  void set_y(const float y);
  void set_vx(const float vx);
  void set_vy(const float vy);
  void set_velocity(const Vector2 v);
  void set_acceleration(const Vector2 a);
  void set_is_animating(const bool is_animating);
  void set_is_flipped(const bool is_flipped);
  void set_type(const sprite_type t);
  void set_ax(const float ax);
  void set_ay(const float ay);
  void set_context(const int n);
  void flip();
  void update(const Vector2 dungeon_position);
  void set_rotation_angle(const float angle);
  void set_is_spinning(const bool is_spinning);
  void set_rotation_speed(const float speed);
  void set_movement_type(const movement_type m);
  void set_alpha(const unsigned int alpha);

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

  void init_rects();
  void init_rects(const float w, const float h);
};
