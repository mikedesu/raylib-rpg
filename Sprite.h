#pragma once

#include "MovementType.h"
#include "SpriteType.h"
#include "mPrint.h"
#include "raylib.h"

typedef struct {

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
    SpriteType type = SPRITETYPE_NONE;
    MovementType movement = MOVEMENT_TYPE_NONE;

} Sprite;

void Sprite_create(Sprite& sprite,
                   Texture2D& t,
                   const unsigned int frames,
                   const unsigned int c,
                   const float x,
                   const float y,
                   const int w,
                   const int h,
                   SpriteType spritetype);
void Sprite_destroy(Sprite& sprite);
void Sprite_draw(Sprite& sprite);
void Sprite_draw_hitbox(Sprite& sprite);
void Sprite_move(Sprite& sprite, const float x, const float y);
void Sprite_move_rect(Sprite& sprite, Rectangle& r);
void Sprite_mark_for_deletion(Sprite& sprite);
void Sprite_incr_frame(Sprite& sprite);
void Sprite_incr_ax(Sprite& sprite, const float ax);
void Sprite_incr_ay(Sprite& sprite, const float ay);
void Sprite_incr_vx(Sprite& sprite, const float vx);
void Sprite_incr_vy(Sprite& sprite, const float vy);
void Sprite_set_scale(Sprite& sprite, const float s);
void Sprite_set_current_frame(Sprite& sprite, const unsigned int frame);
void Sprite_set_x(Sprite& sprite, const float x);
void Sprite_set_y(Sprite& sprite, const float y);
void Sprite_set_vx(Sprite& sprite, const float vx);
void Sprite_set_vy(Sprite& sprite, const float vy);
void Sprite_set_velocity(Sprite& sprite, const Vector2 v);
void Sprite_set_acceleration(Sprite& sprite, const Vector2 a);
void Sprite_set_is_animating(Sprite& sprite, const bool is_animating);
void Sprite_set_is_flipped(Sprite& sprite, const bool is_flipped);
void Sprite_set_type(Sprite& sprite, const SpriteType t);
void Sprite_set_ax(Sprite& sprite, const float ax);
void Sprite_set_ay(Sprite& sprite, const float ay);
void Sprite_set_context(Sprite& sprite, const int n);
void Sprite_flip(Sprite& sprite);
void Sprite_update(Sprite& sprite, const Vector2 dungeon_position);
void Sprite_set_rotation_angle(Sprite& sprite, const float angle);
void Sprite_set_is_spinning(Sprite& sprite, const bool is_spinning);
void Sprite_set_rotation_speed(Sprite& sprite, const float speed);
void Sprite_set_movement_type(Sprite& sprite, const MovementType m);
void Sprite_set_alpha(Sprite& sprite, const unsigned int alpha);
const Rectangle Sprite_get_dest(Sprite& sprite);
const Rectangle Sprite_get_hitbox(Sprite& sprite);
const unsigned int Sprite_get_alpha(Sprite& sprite);
const int Sprite_get_width(Sprite& sprite);
const int Sprite_get_height(Sprite& sprite);
const int Sprite_get_anim_frames(Sprite& sprite);
const int Sprite_get_current_frame(Sprite& sprite);
const int Sprite_get_context(Sprite& sprite);
const int Sprite_get_contexts(Sprite& sprite);
const float Sprite_get_vx(Sprite& sprite);
const float Sprite_get_vy(Sprite& sprite);
const float Sprite_get_ax(Sprite& sprite);
const float Sprite_get_ay(Sprite& sprite);
const float Sprite_get_x(Sprite& sprite);
const float Sprite_get_y(Sprite& sprite);
const float Sprite_get_scale(Sprite& sprite);
const float Sprite_get_rotation_angle(Sprite& sprite);
const float Sprite_get_rotation_speed(Sprite& sprite);
const Vector2 Sprite_get_velocity(Sprite& sprite);
const Vector2 Sprite_get_acceleration(Sprite& sprite);
const bool Sprite_get_is_marked_for_deletion(Sprite& sprite);
const bool Sprite_get_is_animating(Sprite& sprite);
const bool Sprite_get_is_flipped(Sprite& sprite);
const SpriteType Sprite_get_type(Sprite& sprite);
const bool Sprite_get_is_spinning(Sprite& sprite);
void Sprite_init_rects(Sprite& sprite, const int w, const int h);
void Sprite_init_rects(Sprite& sprite, const float w, const float h);
