#include "Sprite.h"
// #include "raymath.h"
#include "rlgl.h"
// #include <cassert>
#include <cstring>

Sprite::Sprite(const char *filepath, const unsigned int frames, const float x,
               const float y, sprite_type t) {
  if (strlen(filepath) > 0) {
    texture = LoadTexture(filepath);
  }

  anim_frames = frames;
  set_scale(1.0f);
  init_rects();
  origin = (Vector2){0, 0};
  dest.x = x;
  dest.y = y;
  current_frame = 0;
  velocity = (Vector2){0, 0};
  is_marked_for_deletion = false;
  is_animating = false;
  is_flipped = false;
  type = t;
  is_spinning = false;
  velocity.x = 0;
  velocity.y = 0;
  acceleration.x = 0;
  acceleration.y = 0;
}

Sprite::Sprite(Texture2D &t, const unsigned int frames, const float x,
               const float y, sprite_type spritetype) {
  texture = t;
  anim_frames = frames;
  set_scale(1.0f);
  init_rects();
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

void Sprite::init_rects() {
  src = (Rectangle){0, 0, (float)texture.width / anim_frames,
                    (float)texture.height};
  flipped_src = (Rectangle){src.x, src.y, src.width * -1.0f, src.height};
  dest =
      (Rectangle){dest.x, dest.y, ((float)texture.width / anim_frames) * scale,
                  (float)texture.height * scale};
  hitbox = (Rectangle){dest.x, dest.y, dest.width, dest.height};
}

Sprite::~Sprite() {}

void Sprite::draw() {
  const Color color = WHITE;
  rlPushMatrix();
  rlTranslatef(dest.x + dest.width / 2, dest.y + dest.height / 2, 0);
  rlRotatef(rotation_angle, 0, 0, 1);
  // rlTranslatef(0, 0, 0);
  rlTranslatef(-dest.x - dest.width / 2, -dest.y - dest.height / 2, 0);
  if (is_flipped) {
    DrawTexturePro(texture, flipped_src, dest, origin, 0.0f, color);
  } else {
    DrawTexturePro(texture, src, dest, origin, 0.0f, color);
  }
  rlPopMatrix();
  const int frame_freq = 10;
  if (is_animating && frame_counter % frame_freq == 0) {
    incr_frame();
  }
  frame_counter++;
}

void Sprite::draw_hitbox() {
  rlPushMatrix();
  rlTranslatef(dest.x + dest.width / 2, dest.y + dest.height / 2, 0);
  rlRotatef(rotation_angle, 0, 0, 1);
  rlTranslatef(-dest.x - dest.width / 2, -dest.y - dest.height / 2, 0);
  // draw the dest box
  DrawRectangleLines(dest.x, dest.y, dest.width, dest.height, GREEN);
  rlPopMatrix();
  // DrawRectangleLines(dest.x, dest.y, dest.width, dest.height, BLUE);
}

void Sprite::move(const float x, const float y) {
  dest.x += x * scale;
  dest.y += y * scale;
}

void Sprite::move_rect(Rectangle &r) {
  dest.x = r.x;
  dest.y = r.y;
}

void Sprite::set_scale(const float s) {
  scale = s;
  init_rects();
}

void Sprite::set_current_frame(const unsigned int frame) {
  if (frame >= anim_frames) {
    return;
  }
  current_frame = frame;
  // update the src rectangle
  src.x = (float)frame * src.width;
  src.y = 0;
  flipped_src.x = src.x;
}

void Sprite::incr_frame() {
  current_frame++;
  if (current_frame >= anim_frames) {
    current_frame = 0;
  }
  src.x = (float)current_frame * src.width;
  flipped_src.x = src.x;
}

const bool Sprite::get_is_marked_for_deletion() const {
  return is_marked_for_deletion;
}

void Sprite::update() {
  // update the velocity
  // if (movement == movement_type::MOVEMENT_TYPE_NORMAL) {
  // dest.x += velocity.x;
  // dest.y += velocity.y;
  //}
  velocity.x += acceleration.x;
  velocity.y += acceleration.y;
  // origin = (Vector2){0, 0};
  if (is_spinning) {
    rotation_angle += rotation_speed;
  }
}

void Sprite::decr_hp(const int dmg) {
  if (hp - dmg <= 0) {
    hp = 0;
  } else {
    hp -= dmg;
  }
}

void Sprite::incr_hp(const int dmg) {
  if (hp + dmg >= maxhp) {
    hp = maxhp;
  } else {
    hp += dmg;
  }
}

void Sprite::mark_for_deletion() { is_marked_for_deletion = true; }
void Sprite::set_ax(const float ax) { acceleration.x = ax; }
void Sprite::set_ay(const float ay) { acceleration.y = ay; }
void Sprite::set_vx(const float vx) { velocity.x = vx; }
void Sprite::set_vy(const float vy) { velocity.y = vy; }
void Sprite::set_type(const sprite_type t) { type = t; }
void Sprite::set_x(const float x) { dest.x = x; }
void Sprite::set_y(const float y) { dest.y = y; }
void Sprite::set_velocity(const Vector2 v) { velocity = v; }
void Sprite::set_acceleration(const Vector2 a) { acceleration = a; }
void Sprite::incr_vx(const float vx) { velocity.x += vx; }
void Sprite::incr_vy(const float vy) { velocity.y += vy; }
void Sprite::incr_ax(const float ax) { acceleration.x += ax; }
void Sprite::incr_ay(const float ay) { acceleration.y += ay; }
const float Sprite::get_ax() const { return acceleration.x; }
const float Sprite::get_ay() const { return acceleration.y; }
const float Sprite::get_scale() const { return scale; }
const float Sprite::get_vx() const { return velocity.x; }
const float Sprite::get_vy() const { return velocity.y; }
const bool Sprite::get_is_flipped() const { return is_flipped; }
const sprite_type Sprite::get_type() const { return type; }
const Vector2 Sprite::get_velocity() const { return velocity; }
const Vector2 Sprite::get_acceleration() const { return acceleration; }
const float Sprite::get_x() const { return dest.x; }
const float Sprite::get_y() const { return dest.y; };
const Rectangle Sprite::get_dest() const { return dest; }
const int Sprite::get_width() const { return src.width * scale; }
const int Sprite::get_height() const { return src.height * scale; }
const int Sprite::get_anim_frames() const { return anim_frames; }
const int Sprite::get_current_frame() const { return current_frame; }
const bool Sprite::get_is_animating() const { return is_animating; }
void Sprite::flip() { set_is_flipped(!get_is_flipped()); }
void Sprite::set_hp(const int hp) { this->hp = hp; }
void Sprite::set_maxhp(const int maxhp) { this->maxhp = maxhp; }
const int Sprite::get_hp() const { return hp; }
const int Sprite::get_maxhp() const { return maxhp; }
void Sprite::set_rotation_angle(const float angle) { rotation_angle = angle; }
const float Sprite::get_rotation_angle() const { return rotation_angle; }
const bool Sprite::get_is_spinning() const { return is_spinning; }
const Rectangle Sprite::get_hitbox() const { return hitbox; }
void Sprite::set_rotation_speed(const float speed) { rotation_speed = speed; }
const float Sprite::get_rotation_speed() const { return rotation_speed; }
void Sprite::set_movement_type(const movement_type m) { movement = m; }
const movement_type Sprite::get_movement_type() const { return movement; }
void Sprite::set_is_spinning(const bool is) { is_spinning = is; }
void Sprite::set_is_animating(const bool is) { is_animating = is; }
void Sprite::set_is_flipped(const bool f) { is_flipped = f; }

unsigned int Sprite::get_alpha() const { return alpha; }
void Sprite::set_alpha(const unsigned int a) { alpha = a; }
