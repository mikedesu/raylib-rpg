#include "Sprite.h"

Sprite::Sprite(Texture2D& t,
               const unsigned int frames,
               const unsigned int c,
               const float x,
               const float y,
               const int w,
               const int h,
               SpriteType spritetype) {

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

Sprite::~Sprite() { }

void Sprite::draw() {
    const Color color = WHITE;
    const float angle = 0.0f;
    //  rlPushMatrix();
    //  rlTranslatef(dest.x + dest.width / 2, dest.y + dest.height / 2, 0);
    //  rlRotatef(rotation_angle, 0, 0, 1);
    //   rlTranslatef(0, 0, 0);
    //  rlTranslatef(-dest.x - dest.width / 2, -dest.y - dest.height / 2, 0);
    if(is_flipped) {
        DrawTexturePro(texture, flipped_src, dest, origin, angle, color);
    } else {
        DrawTexturePro(texture, src, dest, origin, angle, color);
    }
    // rlPopMatrix();
    const int frame_freq = 10;
    if(is_animating && frame_counter % frame_freq == 0) {
        incr_frame();
    }
    frame_counter++;
}

void Sprite::draw_hitbox() {
    DrawRectangleLines(hitbox.x, hitbox.y, hitbox.width, hitbox.height, RED);
}
void Sprite::move(const float x, const float y) {
    dest.x += x * scale;
    dest.y += y * scale;
}
void Sprite::move_rect(Rectangle& r) {
    dest.x = r.x;
    dest.y = r.y;
}
void Sprite::mark_for_deletion() {
    is_marked_for_deletion = true;
}
void Sprite::incr_frame() {
    current_frame++;
    if(current_frame >= anim_frames) {
        current_frame = 0;
    }
    src.x = (float)current_frame * src.width;
    flipped_src.x = src.x;
}
void Sprite::incr_ax(const float ax) {
    acceleration.x += ax;
}
void Sprite::incr_ay(const float ay) {
    acceleration.y += ay;
}
void Sprite::incr_vx(const float vx) {
    velocity.x += vx;
}
void Sprite::incr_vy(const float vy) {
    velocity.y += vy;
}
void Sprite::set_scale(const float s) {
    scale = s;
    dest.width = (float)width * scale;
    dest.height = (float)height * scale;
    // init_rects(width, height);
}
void Sprite::set_current_frame(const unsigned int frame) {
    if(frame >= anim_frames) {
        return;
    }
    current_frame = frame;
    // update the src rectangle
    src.x = (float)frame * src.width;
    src.y = 0;
    flipped_src.x = src.x;
}
void Sprite::set_x(const float x) {
    dest.x = x;
}
void Sprite::set_y(const float y) {
    dest.y = y;
}
void Sprite::set_vx(const float vx) {
    velocity.x = vx;
}
void Sprite::set_vy(const float vy) {
    velocity.y = vy;
}
void Sprite::set_velocity(const Vector2 v) {
    velocity = v;
}
void Sprite::set_acceleration(const Vector2 a) {
    acceleration = a;
}
void Sprite::set_is_animating(const bool is_animating) {
    this->is_animating = is_animating;
}
void Sprite::set_is_flipped(const bool is_flipped) {
    this->is_flipped = is_flipped;
}
void Sprite::set_type(const SpriteType t) {
    type = t;
}
void Sprite::set_ax(const float ax) {
    acceleration.x = ax;
}
void Sprite::set_ay(const float ay) {
    acceleration.y = ay;
}
void Sprite::set_context(const int n) {
    if(n >= contexts) {
        mPrint("Error: context out of bounds.");
        context = 0;
    } else {
        context = n;
    }
    // whenever the context is updated, we have to re-initialize our rects
    init_rects(width, height);
}
void Sprite::flip() {
    is_flipped = !is_flipped;
}

void Sprite::update(const Vector2 dungeon_position) {
    const int tilesize = 20;
    set_x(dungeon_position.x * tilesize * scale);
    set_y(dungeon_position.y * tilesize * scale);
    if(is_spinning) {
        rotation_angle += rotation_speed;
    }
}

void Sprite::set_rotation_angle(const float angle) {
    rotation_angle = angle;
}

void Sprite::set_is_spinning(const bool is_spinning) {
    this->is_spinning = is_spinning;
}
void Sprite::set_rotation_speed(const float speed) {
    rotation_speed = speed;
}
void Sprite::set_movement_type(const MovementType m) {
    movement = m;
}
void Sprite::set_alpha(const unsigned int alpha) {
    this->alpha = alpha;
}

const Rectangle Sprite::get_dest() const {
    return dest;
}
const Rectangle Sprite::get_hitbox() const {
    return hitbox;
}
const unsigned int Sprite::get_alpha() const {
    return alpha;
}
const int Sprite::get_width() const {
    return width;
}
const int Sprite::get_height() const {
    return height;
}
const int Sprite::get_anim_frames() const {
    return anim_frames;
}
const int Sprite::get_current_frame() const {
    return current_frame;
}
const int Sprite::get_context() const {
    return context;
}
const int Sprite::get_contexts() const {
    return contexts;
}
const float Sprite::get_vx() const {
    return velocity.x;
}
const float Sprite::get_vy() const {
    return velocity.y;
}
const float Sprite::get_ax() const {
    return acceleration.x;
}
const float Sprite::get_ay() const {
    return acceleration.y;
}
const float Sprite::get_x() const {
    return dest.x;
}
const float Sprite::get_y() const {
    return dest.y;
}
const float Sprite::get_scale() const {
    return scale;
}
const float Sprite::get_rotation_angle() const {
    return rotation_angle;
}
const float Sprite::get_rotation_speed() const {
    return rotation_speed;
}
const Vector2 Sprite::get_velocity() const {
    return velocity;
}
const Vector2 Sprite::get_acceleration() const {
    return acceleration;
}
const bool Sprite::get_is_marked_for_deletion() const {
    return is_marked_for_deletion;
}
const bool Sprite::get_is_animating() const {
    return is_animating;
}
const bool Sprite::get_is_flipped() const {
    return is_flipped;
}
const SpriteType Sprite::get_type() const {
    return type;
}
const bool Sprite::get_is_spinning() const {
    return is_spinning;
}

void Sprite::init_rects() {
    src = (Rectangle){0, 0, (float)texture.width / anim_frames, (float)texture.height};
    flipped_src = (Rectangle){src.x, src.y, src.width * -1.0f, src.height};
    dest = (Rectangle){dest.x,
                       dest.y,
                       ((float)texture.width / anim_frames) * scale,
                       (float)texture.height * scale};
    hitbox = (Rectangle){dest.x, dest.y, dest.width, dest.height};
}

void Sprite::init_rects(const float w, const float h) {

    src = (Rectangle){0, h * context, w, h};
    flipped_src = (Rectangle){src.x, src.y, w * -1.0f, h};
    dest = (Rectangle){dest.x, dest.y, w * scale, h * scale};
    hitbox = (Rectangle){dest.x, dest.y, dest.width, dest.height};
}
