#include "Sprite.h"

void Sprite_create(Sprite& sprite,
                   Texture2D& t,
                   const unsigned int frames,
                   const unsigned int c,
                   const float x,
                   const float y,
                   const int w,
                   const int h,
                   SpriteType spritetype) {

    sprite.texture = t;
    sprite.anim_frames = frames;
    sprite.contexts = c;
    sprite.width = w;
    sprite.height = h;
    Sprite_set_scale(sprite, 1.0f);
    Sprite_init_rects(sprite, w, h);
    sprite.origin = (Vector2){0, 0};
    sprite.dest.x = x;
    sprite.dest.y = y;
    sprite.current_frame = 0;
    sprite.velocity = (Vector2){0, 0};
    sprite.is_marked_for_deletion = false;
    sprite.is_animating = false;
    sprite.is_flipped = false;
    sprite.type = spritetype;
    sprite.is_spinning = false;
    sprite.velocity.x = 0;
    sprite.velocity.y = 0;
    sprite.acceleration.x = 0;
    sprite.acceleration.y = 0;
}

void Sprite_destroy(Sprite& sprite) { }

void Sprite_draw(Sprite& sprite) {
    const Color color = WHITE;
    const float angle = 0.0f;
    //  rlPushMatrix();
    //  rlTranslatef(dest.x + dest.width / 2, dest.y + dest.height / 2, 0);
    //  rlRotatef(rotation_angle, 0, 0, 1);
    //   rlTranslatef(0, 0, 0);
    //  rlTranslatef(-dest.x - dest.width / 2, -dest.y - dest.height / 2, 0);
    if(sprite.is_flipped) {
        DrawTexturePro(
            sprite.texture, sprite.flipped_src, sprite.dest, sprite.origin, angle, color);
    } else {
        DrawTexturePro(sprite.texture, sprite.src, sprite.dest, sprite.origin, angle, color);
    }
    // rlPopMatrix();
    const int frame_freq = 10;
    if(sprite.is_animating && sprite.frame_counter % frame_freq == 0) {
        //incr_frame();
        Sprite_incr_frame(sprite);
    }
    sprite.frame_counter++;
}

void Sprite_draw_hitbox(Sprite& sprite) {
    DrawRectangleLines(
        sprite.hitbox.x, sprite.hitbox.y, sprite.hitbox.width, sprite.hitbox.height, RED);
}
void Sprite_move(Sprite& sprite, const float x, const float y) {
    sprite.dest.x += x * sprite.scale;
    sprite.dest.y += y * sprite.scale;
}
void Sprite_move_rect(Sprite& sprite, Rectangle& r) {
    sprite.dest.x = r.x;
    sprite.dest.y = r.y;
}
void Sprite_mark_for_deletion(Sprite& sprite) {
    sprite.is_marked_for_deletion = true;
}
void Sprite_incr_frame(Sprite& sprite) {
    sprite.current_frame++;
    if(sprite.current_frame >= sprite.anim_frames) {
        sprite.current_frame = 0;
    }
    sprite.src.x = (float)sprite.current_frame * sprite.src.width;
    sprite.flipped_src.x = sprite.src.x;
}
void Sprite_incr_ax(Sprite& sprite, const float ax) {
    sprite.acceleration.x += ax;
}
void Sprite_incr_ay(Sprite& sprite, const float ay) {
    sprite.acceleration.y += ay;
}
void Sprite_incr_vx(Sprite& sprite, const float vx) {
    sprite.velocity.x += vx;
}
void Sprite_incr_vy(Sprite& sprite, const float vy) {
    sprite.velocity.y += vy;
}
void Sprite_set_scale(Sprite& sprite, const float s) {
    sprite.scale = s;
    sprite.dest.width = (float)sprite.width * sprite.scale;
    sprite.dest.height = (float)sprite.height * sprite.scale;
    Sprite_init_rects(sprite, sprite.dest.width, sprite.dest.height);
}
void Sprite_set_current_frame(Sprite& sprite, const unsigned int frame) {
    if(frame >= sprite.anim_frames) {
        return;
    }
    sprite.current_frame = frame;
    // update the src rectangle
    sprite.src.x = (float)frame * sprite.src.width;
    sprite.src.y = 0;
    sprite.flipped_src.x = sprite.src.x;
}
void Sprite_set_x(Sprite& sprite, const float x) {
    sprite.dest.x = x;
    sprite.hitbox.x = x;
}
void Sprite_set_y(Sprite& sprite, const float y) {
    sprite.dest.y = y;
    sprite.hitbox.y = y;
}
void Sprite_set_vx(Sprite& sprite, const float vx) {
    sprite.velocity.x = vx;
}
void Sprite_set_vy(Sprite& sprite, const float vy) {
    sprite.velocity.y = vy;
}
void Sprite_set_velocity(Sprite& sprite, const Vector2 v) {
    sprite.velocity = v;
}
void Sprite_set_acceleration(Sprite& sprite, const Vector2 a) {
    sprite.acceleration = a;
}
void Sprite_set_is_animating(Sprite& sprite, const bool is_animating) {
    sprite.is_animating = is_animating;
}
void Sprite_set_is_flipped(Sprite& sprite, const bool is_flipped) {
    sprite.is_flipped = is_flipped;
}
void Sprite_set_type(Sprite& sprite, const SpriteType t) {
    sprite.type = t;
}
void Sprite_set_ax(Sprite& sprite, const float ax) {
    sprite.acceleration.x = ax;
}
void Sprite_set_ay(Sprite& sprite, const float ay) {
    sprite.acceleration.y = ay;
}
void Sprite_set_context(Sprite& sprite, const int n) {
    if(n >= sprite.contexts) {
        mPrint("Error: context out of bounds.");
        sprite.context = 0;
    } else {
        sprite.context = n;
    }
    // whenever the context is updated, we have to re-initialize our rects
    Sprite_init_rects(sprite, sprite.width, sprite.height);
}
void Sprite_flip(Sprite& sprite) {
    sprite.is_flipped = !sprite.is_flipped;
}

void Sprite_update(Sprite& sprite, const Vector2 dungeon_position) {
    const int tilesize = 20;
    Sprite_set_x(sprite, dungeon_position.x * tilesize * sprite.scale);
    Sprite_set_y(sprite, dungeon_position.y * tilesize * sprite.scale);
    if(sprite.is_spinning) {
        sprite.rotation_angle += sprite.rotation_speed;
    }
}

void Sprite_set_rotation_angle(Sprite& sprite, const float angle) {
    sprite.rotation_angle = angle;
}

void Sprite_set_is_spinning(Sprite& sprite, const bool is_spinning) {
    sprite.is_spinning = is_spinning;
}
void Sprite_set_rotation_speed(Sprite& sprite, const float speed) {
    sprite.rotation_speed = speed;
}
void Sprite_set_movement_type(Sprite& sprite, const MovementType m) {
    sprite.movement = m;
}
void Sprite_set_alpha(Sprite& sprite, const unsigned int alpha) {
    sprite.alpha = alpha;
}

const Rectangle Sprite_get_dest(Sprite& sprite) {
    return sprite.dest;
}
const Rectangle Sprite_get_hitbox(Sprite& sprite) {
    return sprite.hitbox;
}
const unsigned int Sprite_get_alpha(Sprite& sprite) {
    return sprite.alpha;
}
const int Sprite_get_width(Sprite& sprite) {
    return sprite.width;
}
const int Sprite_get_height(Sprite& sprite) {
    return sprite.height;
}
const int Sprite_get_anim_frames(Sprite& sprite) {
    return sprite.anim_frames;
}
const int Sprite_get_current_frame(Sprite& sprite) {
    return sprite.current_frame;
}
const int Sprite_get_context(Sprite& sprite) {
    return sprite.context;
}
const int Sprite_get_contexts(Sprite& sprite) {
    return sprite.contexts;
}
const float Sprite_get_vx(Sprite& sprite) {
    return sprite.velocity.x;
}
const float Sprite_get_vy(Sprite& sprite) {
    return sprite.velocity.y;
}
const float Sprite_get_ax(Sprite& sprite) {
    return sprite.acceleration.x;
}
const float Sprite_get_ay(Sprite& sprite) {
    return sprite.acceleration.y;
}
const float Sprite_get_x(Sprite& sprite) {
    return sprite.dest.x;
}
const float Sprite_get_y(Sprite& sprite) {
    return sprite.dest.y;
}
const float Sprite_get_scale(Sprite& sprite) {
    return sprite.scale;
}
const float Sprite_get_rotation_angle(Sprite& sprite) {
    return sprite.rotation_angle;
}
const float Sprite_get_rotation_speed(Sprite& sprite) {
    return sprite.rotation_speed;
}
const Vector2 Sprite_get_velocity(Sprite& sprite) {
    return sprite.velocity;
}
const Vector2 Sprite_get_acceleration(Sprite& sprite) {
    return sprite.acceleration;
}
const bool Sprite_get_is_marked_for_deletion(Sprite& sprite) {
    return sprite.is_marked_for_deletion;
}
const bool Sprite_get_is_animating(Sprite& sprite) {
    return sprite.is_animating;
}
const bool Sprite_get_is_flipped(Sprite& sprite) {
    return sprite.is_flipped;
}
const SpriteType Sprite_get_type(Sprite& sprite) {
    return sprite.type;
}
const bool Sprite_get_is_spinning(Sprite& sprite) {
    return sprite.is_spinning;
}

void Sprite_init_rects(Sprite& sprite, const int w, const int h) {
    sprite.src = (Rectangle){
        0, 0, (float)sprite.texture.width / sprite.anim_frames, (float)sprite.texture.height};
    sprite.flipped_src =
        (Rectangle){sprite.src.x, sprite.src.y, sprite.src.width * -1.0f, sprite.src.height};
    sprite.dest = (Rectangle){sprite.dest.x,
                              sprite.dest.y,
                              ((float)sprite.texture.width / sprite.anim_frames) * sprite.scale,
                              (float)sprite.texture.height * sprite.scale};
    sprite.hitbox =
        (Rectangle){sprite.dest.x, sprite.dest.y, sprite.dest.width, sprite.dest.height};
}

void Sprite_init_rects(Sprite& sprite, const float w, const float h) {

    sprite.src = (Rectangle){0, h * sprite.context, w, h};
    sprite.flipped_src = (Rectangle){sprite.src.x, sprite.src.y, w * -1.0f, h};
    sprite.dest = (Rectangle){sprite.dest.x, sprite.dest.y, w * sprite.scale, h * sprite.scale};
    sprite.hitbox =
        (Rectangle){sprite.dest.x, sprite.dest.y, sprite.dest.width, sprite.dest.height};
}
