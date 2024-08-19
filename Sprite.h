#pragma once

#include "movement_type.h"
#include "raylib.h"

typedef enum {
  SPRITETYPE_NONE,
  SPRITETYPE_TITLE,
  SPRITETYPE_PLAYER,
  SPRITETYPE_ENEMY,
  SPRITETYPE_ITEM,
  // SPRITETYPE_SOULSHARD,
  // SPRITETYPE_POWERUP_HEART,
  // SPRITETYPE_TILE,
  //  SPRITETYPE_KNIFE,
  //   SPRITETYPE_PIPEBASE,
  //   SPRITETYPE_RED_BRICK,
  //   SPRITETYPE_BULLET,
  SPRITETYPE_COUNT
} sprite_type;

class Sprite {

public:
  Sprite(const char *filepath, const unsigned int anim_frames, const float x,
         const float y, sprite_type t);
  Sprite(Texture2D &texture, const unsigned int anim_frames, const float x,
         const float y, sprite_type t);
  Sprite(Texture2D &texture, const unsigned int anim_frames, const float x,
         const float y, const int w, const int h, sprite_type t);

  ~Sprite();

  void draw();
  void draw_hitbox();
  void move(const float x, const float y);
  void move_rect(Rectangle &r);
  void mark_for_deletion();

  const float get_scale() const;
  const float get_x() const;
  const float get_y() const;
  const Rectangle get_dest() const;
  const Rectangle get_hitbox() const;
  const int get_width() const;
  const int get_height() const;
  const int get_anim_frames() const;
  const int get_current_frame() const;
  const float get_vx() const;
  const float get_vy() const;
  const Vector2 get_velocity() const;
  const Vector2 get_acceleration() const;
  const bool get_is_marked_for_deletion() const;
  const bool get_is_animating() const;
  const bool get_is_flipped() const;
  const sprite_type get_type() const;
  const float get_ax() const;
  const float get_ay() const;
  // const int get_hp() const;
  // const int get_maxhp() const;
  const bool get_is_spinning() const;
  const float get_rotation_angle() const;
  const float get_rotation_speed() const;
  // const movement_type get_movement_type() const;
  // const Vector2 get_dungeon_position() const;

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
  // void set_hp(const int hp);
  // void set_maxhp(const int maxhp);
  // void decr_hp(const int hp);
  // void incr_hp(const int hp);
  void flip();
  void update(const Vector2 dungeon_position);
  void set_rotation_angle(const float angle);
  void set_is_spinning(const bool is_spinning);
  void set_rotation_speed(const float speed);
  void set_movement_type(const movement_type m);

  const unsigned int get_alpha() const;
  void set_alpha(const unsigned int alpha);

  // void set_dungeon_position(const Vector2 pos);

  // void incr_dungeon_position_x(const float x);
  // void incr_dungeon_position_y(const float y);

private:
  Texture2D texture;

  Rectangle src;
  Rectangle flipped_src;
  Rectangle dest;
  Rectangle hitbox = {0, 0, 0, 0};

  unsigned int anim_frames;
  unsigned int current_frame;
  unsigned int frame_counter;
  // unsigned int hp;
  // unsigned int maxhp;

  float scale = 1.0f;
  float rotation_angle = 0.0f;
  float rotation_speed = 1.0f;

  Vector2 origin;
  Vector2 velocity;
  Vector2 acceleration;

  // Vector2 dungeon_position;

  bool is_marked_for_deletion;
  bool is_animating;
  bool is_flipped;
  bool is_spinning = false;

  sprite_type type;

  movement_type movement = MOVEMENT_TYPE_NONE;

  unsigned int alpha;

  void init_rects();
  void init_rects(const float w, const float h);
};
