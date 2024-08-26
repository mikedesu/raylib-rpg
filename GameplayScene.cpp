#include "GameplayScene.h"
#include "entity_type.h"
#include "mPrint.h"
#include "raymath.h"

GameplayScene::GameplayScene() {
  mPrint("GameplayScene constructor");
  last_mouse_click_pos = (Vector2){0, 0};
  last_tile_click_pos = (Vector2){-1, -1};
  set_control_mode(CONTROL_MODE_PLAYER);
  set_texture_filepath("game_textures.txt");
  set_global_scale(3.0f);
  set_scene_transition(SCENE_TRANSITION_IN);
  set_scene_type(SCENE_TYPE_GAMEPLAY);
}

GameplayScene::~GameplayScene() { mPrint("GameplayScene destructor"); }

void GameplayScene::gameover() { set_scene_transition(SCENE_TRANSITION_OUT); }

void GameplayScene::update() {
  // we will need to update the relative onscreen positions of the sprites
  // depending on
  // 1. position in the dungeon
  // 2. global scale
  for (auto &s : get_sprites()) {
    Vector2 dungeon_pos = dungeon_floor.get_entity_position(s.first);
    s.second->update(dungeon_pos);
  }
  if (player_attempted_move) {
    turn_count++;
    player_attempted_move = false;
  }
}

inline void GameplayScene::handle_camera_input_move() {
  if (IsKeyDown(KEY_UP)) {
    get_camera2d().target.y -= 10;
  }
  if (IsKeyDown(KEY_DOWN)) {
    get_camera2d().target.y += 10;
  }
  if (IsKeyDown(KEY_LEFT)) {
    get_camera2d().target.x -= 10;
  }
  if (IsKeyDown(KEY_RIGHT)) {
    get_camera2d().target.x += 10;
  }
}

inline void GameplayScene::handle_camera_input_zoom() {
  const float zoom_incr = 1;
  if (IsKeyDown(KEY_RIGHT_SHIFT) && IsKeyPressed(KEY_Z)) {
    set_scale(get_global_scale() - zoom_incr);
    prev_tile_click_zoom_level = tile_click_zoom_level;
    tile_click_zoom_level = get_global_scale();
    last_mouse_click_pos = (Vector2){-1, -1};
  } else if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_Z)) {
    set_scale(get_global_scale() - zoom_incr);
    prev_tile_click_zoom_level = tile_click_zoom_level;
    tile_click_zoom_level = get_global_scale();
    last_mouse_click_pos = (Vector2){-1, -1};
  } else if (IsKeyPressed(KEY_Z)) {
    set_scale(get_global_scale() + zoom_incr);
    prev_tile_click_zoom_level = tile_click_zoom_level;
    tile_click_zoom_level = get_global_scale();
    last_mouse_click_pos = (Vector2){-1, -1};
  }
}

inline void GameplayScene::handle_camera_input() {
  handle_camera_input_move();
  handle_camera_input_zoom();
}

bool GameplayScene::handle_dungeon_move_pos(const entity_id id,
                                            const Vector2 c_pos,
                                            const Vector2 t_pos) {
  bool retval = false;
  //  if they are different tiles...
  if (!Vector2Equals(c_pos, t_pos)) {
    tile_type t = dungeon_floor.get_tile_type(t_pos.x, t_pos.y);
    switch (t) {
    case TILE_FLOOR_BASIC:
    case TILE_FLOOR_STONE:
    case TILE_FLOOR_WOOD:
    case TILE_FLOOR_DIRT:
    case TILE_FLOOR_UPSTAIRS:
    case TILE_FLOOR_DOWNSTAIRS: {
      bool r = dungeon_floor.move_entity_to_tile(id, t_pos);
      if (r) {
        dungeon_events.push_back(
            DungeonEvent(id, EVENT_ENTITY_MOVE_SUCCESS, t_pos));
        get_popup_manager()->render("Moved");
        retval = true;
      } else {
        dungeon_events.push_back(
            DungeonEvent(id, EVENT_ENTITY_MOVE_FAIL, t_pos));
        get_popup_manager()->render("Cannot move");
        retval = false;
      }
    } break;
    default:
      dungeon_events.push_back(DungeonEvent(id, EVENT_ENTITY_MOVE_FAIL, t_pos));
      get_popup_manager()->render("Cannot move");
      retval = false;
      break;
    }
  }
  return retval;
}

Vector2 GameplayScene::handle_dungeon_move_dir(const entity_id id,
                                               const Vector2 direction) {
  // we have to check the dungeon position of the sprite
  // if the target tile is a location we cant move to, like none, void, or wall,
  // then we don't move
  // get the current dungeon position of the sprite
  Vector2 cur_pos = dungeon_floor.get_entity_position(id);
  Vector2 t_pos = Vector2Add(cur_pos, direction);
  // if the locations are equal, no move is executed
  // this way be interpeted as a "wait" action in the future
  bool success = handle_dungeon_move_pos(id, cur_pos, t_pos);
  Vector2 retval = (Vector2){0, 0};
  if (success) {
    retval = direction;
  }
  return retval;
}

inline void GameplayScene::handle_player_move_direction() {
  const int tilesize = 20;
  Vector2 move_dir = (Vector2){0, 0};
  // unsigned int context = get_sprite(player_id)->get_context();
  bool is_flipped = false;
  if (IsKeyPressed(KEY_UP)) {
    player_attempted_move = true;
    // set the player sprite's context
    move_dir = handle_dungeon_move_dir(player_id, (Vector2){0, -1});
    get_sprite(player_id)->set_context(1);
    get_sprite(player_id)->set_is_flipped(is_flipped);
  } else if (IsKeyPressed(KEY_DOWN)) {
    player_attempted_move = true;
    move_dir = handle_dungeon_move_dir(player_id, (Vector2){0, 1});
    get_sprite(player_id)->set_context(0);
    get_sprite(player_id)->set_is_flipped(is_flipped);
  } else if (IsKeyPressed(KEY_LEFT)) {
    player_attempted_move = true;
    is_flipped = true;
    move_dir = handle_dungeon_move_dir(player_id, (Vector2){-1, 0});
    get_sprite(player_id)->set_context(2);
    get_sprite(player_id)->set_is_flipped(is_flipped);
  } else if (IsKeyPressed(KEY_RIGHT)) {
    player_attempted_move = true;
    move_dir = handle_dungeon_move_dir(player_id, (Vector2){1, 0});
    get_sprite(player_id)->set_context(2);
    get_sprite(player_id)->set_is_flipped(is_flipped);
  }
  // get_sprite(player_id)->set_context(context);
  //  update the camera
  get_camera2d().target.x += move_dir.x * tilesize * get_global_scale();
  get_camera2d().target.y += move_dir.y * tilesize * get_global_scale();
}

inline void GameplayScene::handle_player_mouse_click() {
  // select a tile on screen
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    Vector2 mouse_pos = GetMousePosition();
    Vector2 world_pos = GetScreenToWorld2D(mouse_pos, get_camera2d());
    last_mouse_click_pos = world_pos;
    tile_is_selected = true;
    // dungeon_floor.get_tile_by_col_row(i, j).increase_light_level();
  }
}

inline void GameplayScene::handle_player_input() {
  handle_player_mouse_click();
  if (IsKeyPressed(KEY_R)) {
    if (Vector2Equals(last_tile_click_pos, (Vector2){-1, -1}))
      return;
    // get the tile at the last clicked tile and get its entities
    vector<entity_id> entities = dungeon_floor.get_entities(
        last_tile_click_pos.x, last_tile_click_pos.y);
    bool can_place = true;

    for (auto entity_id : entities) {
      if (get_sprite(entity_id)->get_type() == SPRITETYPE_WALL) {
        can_place = false;
      }
    }

    if (can_place) {
      spawn_torch(last_tile_click_pos);
      // update the lighting for that tile as well as the surrounding tiles
      // this will require a method that takes in the vector2 position of the
      // tile and then dynamically modify the surrounding tile lighting
      increase_lighting_at(last_tile_click_pos, 3);
    }
  }

  if (IsKeyPressed(KEY_T)) {
    if (Vector2Equals(last_tile_click_pos, (Vector2){-1, -1}))
      return;
    vector<entity_id> entities = dungeon_floor.get_entities(
        last_tile_click_pos.x, last_tile_click_pos.y);
    bool can_place = true;

    for (auto entity_id : entities) {
      if (get_sprite(entity_id)->get_type() == SPRITETYPE_WALL) {
        can_place = false;
      }
    }

    if (can_place) {
      // we must iterate thru the entities on the tile and remove the first
      // torch if we are successful, then we can decrease the lighting
      bool success = false;
      for (auto entity_id : dungeon_floor.get_entities(last_tile_click_pos.x,
                                                       last_tile_click_pos.y)) {

        if (dungeon_floor.get_entity_type(entity_id) == ENTITY_TORCH) {
          dungeon_floor.remove_entity(entity_id);
          success = true;
          break;
        }
      }

      if (success) {
        decrease_lighting_at(last_tile_click_pos, 3);
      }
    }
  }

  handle_player_move_direction();
  prev_tile_click_zoom_level = tile_click_zoom_level;
}

void GameplayScene::increase_lighting_at(const Vector2 loc,
                                         const int light_level) {
  if (loc.x < 0 || loc.y < 0 || loc.x >= dungeon_floor.get_gridsize() ||
      loc.y >= dungeon_floor.get_gridsize()) {
    return;
  }

  if (light_level <= 0) {
    return;
  }

  const int gridsize = dungeon_floor.get_gridsize();
  const int x = loc.x;
  const int y = loc.y;

  for (int j = 0; j < light_level; j++) {
    const int x0 = x - j;
    const int x1 = x + j;
    for (int i = 0; i < light_level - j; i++) {
      const int light_incr = light_level - j - i;
      const int y0 = y - i;
      const int y1 = y + i;
      if (light_incr <= 0) {
        break;
      }

      if (y0 >= 0 && x0 >= 0) {
        dungeon_floor.get_tile_by_col_row(x0, y0).increase_light_level_by(
            light_incr);
      }

      if (y1 < gridsize && x0 >= 0) {
        dungeon_floor.get_tile_by_col_row(x0, y1).increase_light_level_by(
            light_incr);
      }

      if (y0 >= 0 && x1 < gridsize) {
        dungeon_floor.get_tile_by_col_row(x1, y0).increase_light_level_by(
            light_incr);
      }

      if (y1 < gridsize && x1 < gridsize) {
        dungeon_floor.get_tile_by_col_row(x1, y1).increase_light_level_by(
            light_incr);
      }
    }
  }
}

void GameplayScene::decrease_lighting_at(const Vector2 loc,
                                         const int light_level) {
  if (loc.x < 0 || loc.y < 0 || loc.x >= dungeon_floor.get_gridsize() ||
      loc.y >= dungeon_floor.get_gridsize()) {
    return;
  }

  if (light_level <= 0) {
    return;
  }

  const int gridsize = dungeon_floor.get_gridsize();
  const int x = loc.x;
  const int y = loc.y;

  for (int j = 0; j < light_level; j++) {
    const int x0 = x - j;
    const int x1 = x + j;
    for (int i = 0; i < light_level - j; i++) {
      const int light_incr = light_level - j - i;
      const int y0 = y - i;
      const int y1 = y + i;

      if (light_incr <= 0) {
        break;
      }

      if (y0 >= 0 && x0 >= 0) {
        dungeon_floor.get_tile_by_col_row(x0, y0).decrease_light_level_by(
            light_incr);
      }

      if (y1 < gridsize && x0 >= 0) {
        dungeon_floor.get_tile_by_col_row(x0, y1).decrease_light_level_by(
            light_incr);
      }

      if (y0 >= 0 && x1 < gridsize) {
        dungeon_floor.get_tile_by_col_row(x1, y0).decrease_light_level_by(
            light_incr);
      }

      if (y1 < gridsize && x1 < gridsize) {
        dungeon_floor.get_tile_by_col_row(x1, y1).decrease_light_level_by(
            light_incr);
      }
    }
  }
}

inline void GameplayScene::handle_control_mode_switch() {
  control_mode mode = get_control_mode();
  if (IsKeyPressed(KEY_C)) {
    if (mode == CONTROL_MODE_CAMERA) {
      set_control_mode(CONTROL_MODE_PLAYER);
    } else if (mode == CONTROL_MODE_PLAYER) {
      set_control_mode(CONTROL_MODE_CAMERA);
    }
  }
}

void GameplayScene::handle_input() {
  if (IsKeyPressed(KEY_D)) {
    flip_debug_panel();
  }

  if (IsKeyPressed(KEY_F)) {
    ToggleFullscreen();
  }

  handle_control_mode_switch();
  control_mode mode = get_control_mode();
  switch (mode) {
  case CONTROL_MODE_CAMERA:
    handle_camera_input();
    break;
  case CONTROL_MODE_PLAYER:
    handle_player_input();
    break;
  default:
    break;
  }
}

void GameplayScene::set_scale(const float f) {
  if (f > 0) {
    set_global_scale(f);
    for (auto &s : get_sprites()) {
      s.second->set_scale(f);
    }
  }
}

void GameplayScene::init_dungeon_floor() {
  // these are hardcoded values, but we would prefer to do this
  // algorithmically and intelligently the columns, for now, represent walls
  // that sit on top of tiles
  spawn_column((Vector2){2, 2});
  spawn_column((Vector2){2, 3});
  spawn_column((Vector2){2, 4});
  spawn_column((Vector2){2, 5});

  spawn_column((Vector2){3, 2});

  spawn_column((Vector2){4, 2});
  spawn_column((Vector2){4, 3});
  spawn_column((Vector2){4, 5});
}

const Vector2 GameplayScene::get_start_location() const {
  return (Vector2){1, 1};
}

bool GameplayScene::init() {
  if (!get_has_been_initialized()) {
    mPrint("Initializing gameplay scene...");
    mPrint("Initializing camera...");
    set_camera_default_values();
    mPrint("Loading assets...");
    load_fonts();
    bool result = load_textures();
    if (!result) {
      mPrint("Error loading textures. Exiting...");
      return false;
    }
    init_dungeon_floor();
    mPrint("Spawning player...");
    Vector2 start_loc = get_start_location();
    spawn_player(start_loc);
    // set the player's dungeon position
    mPrint("Setting camera target...");
    // we want to lock the camera to the player in the center of the screen
    // get_camera2d().target = dungeon_floor.get_entity_position(player_id);
    // these values are hardcoded, but we would prefer to do this
    // algorithmically
    get_camera2d().target.x = -770;
    get_camera2d().target.y = -400;
    prev_tile_click_zoom_level = get_global_scale();
    tile_click_zoom_level = get_global_scale();
    mPrint("Loading sound effects...");
    set_has_been_initialized(true);
    mPrint("GameplayScene initialized");
  }
  return true;
}

const entity_id GameplayScene::spawn_player(const Vector2 pos) {
  entity_id id =
      spawn_entity("player", 0, 0, SPRITETYPE_PLAYER, true, get_global_scale());
  player_id = id;
  dungeon_floor.set_entity_on_tile_with_type(id, ENTITY_PLAYER, pos);
  return id;
}

const entity_id GameplayScene::spawn_column(const Vector2 pos) {
  entity_id id =
      spawn_entity("column", 0, 0, SPRITETYPE_WALL, true, get_global_scale());
  dungeon_floor.set_entity_on_tile_with_type(id, ENTITY_WALL, pos);
  return id;
}

const entity_id GameplayScene::spawn_torch(const Vector2 pos) {
  entity_id id =
      spawn_entity("torch", 0, 0, SPRITETYPE_ITEM, true, get_global_scale());
  dungeon_floor.set_entity_on_tile_with_type(id, ENTITY_TORCH, pos);
  return id;
}

inline void GameplayScene::draw_debug_panel() {
  string camera_info_str =
      "Current Frame: " + to_string(get_current_frame()) + "\n" +
      "Control mode: " + to_string(get_control_mode()) + "\n" +
      "Camera target: " + to_string(get_camera2d().target.x) + ", " +
      to_string(get_camera2d().target.y) + "\n" + "GameplayScene" +
      "Sprites: " + to_string(get_sprites().size()) + "\n" +
      "Textures: " + to_string(get_textures().size()) + "\n" +
      "IsPaused: " + to_string(get_paused()) + "\n" +
      "Global Scale: " + to_string(get_global_scale()) + "\n" +
      "Tile Click Zoom Level: " + to_string(tile_click_zoom_level) + "\n" +
      "Prev Tile Click Zoom Level: " + to_string(prev_tile_click_zoom_level) +
      "\n";
  const int pad = 10;
  const int w = 500;
  const int h = 300;
  const int x = pad;
  const int y = GetScreenHeight() - h - 80;
  const Color c0 = Fade(BLACK, 0.5f);
  const Color c1 = WHITE;
  const Color c2 = GRAY;
  // const int fontsize = 20;
  const int fontsize = get_global_font().baseSize;
  const float alpha = 0.5f;
  const Vector2 loc = (Vector2){(float)x + pad, (float)y + pad};
  DrawRectangle(x, y, w, h, c0);
  DrawTextEx(get_global_font(), camera_info_str.c_str(), loc, fontsize, alpha,
             c1);
  DrawRectangleLines(x, y, w, h, c2);
}

inline void GameplayScene::draw_controls() {
  const int fontsize = get_global_font().baseSize;
  const int x = 10;
  const int y = 10;
  const int w = 400;
  const int h = 200;
  const int pad = 10;
  const Color c0 = Fade(BLACK, 0.5f);
  const Color c1 = WHITE;
  const Color c2 = GRAY;
  const string s = "Controls: \n"
                   "Arrow keys: move player\n"
                   "D: toggle debug panel\n"
                   "C: toggle player/cam mode\n"
                   "Zz: zoom in/out\n"
                   "Q: quit\n";
  DrawRectangle(x, y, w, h, c0);
  DrawRectangleLines(x, y, w, h, c2);
  DrawTextEx(get_global_font(), s.c_str(),
             (Vector2){(float)x + pad, (float)y + pad}, fontsize, 0.5f, c1);
}

inline void GameplayScene::draw_hud() {
  // draw a black box on the right side of the screen
  const int w = 500;
  const int h = GetScreenHeight() / 4 + 10;
  const float x = GetScreenWidth() - w - 10;
  const int y = 10;
  // const int fontsize = 20;
  const int fontsize = get_global_font().baseSize;
  const Color c0 = Fade(BLACK, 0.5f);
  DrawRectangle(x, y, w, h, c0);
  // get the tile type string of the last tile clicked
  string tile_type_str = "TILE_NONE";
  string tile_light_level_str = "0";
  if (last_tile_click_pos.x >= 0 && last_tile_click_pos.y >= 0) {
    tile_type_str =
        dungeon_floor
            .get_tile_by_col_row(last_tile_click_pos.x, last_tile_click_pos.y)
            .get_type_str();
    tile_light_level_str = to_string(
        dungeon_floor
            .get_tile_by_col_row(last_tile_click_pos.x, last_tile_click_pos.y)
            .get_light_level());
  }

  // draw some text
  string s = "";
  s += "Name: darkmage\n";
  s += "Level: 1\n";
  s += "Race: human\n";
  s += "Class: wizard\n";
  s += "HP: 1/1\n";
  s += "Stats: [10,10,10,10,10,10]\n";
  s += "Position: " +
       to_string((int)dungeon_floor.get_entity_position(player_id).x) + ", " +
       to_string((int)dungeon_floor.get_entity_position(player_id).y) + "\n";
  s += "Camera: " + to_string((int)get_camera2d().target.x) + ", " +
       to_string((int)get_camera2d().target.y) + "\n";
  s += "Turn: " + to_string(turn_count) + "\n";
  s += "Last Tile Click: " + to_string((int)last_tile_click_pos.x) + ", " +
       to_string((int)last_tile_click_pos.y) + "\n";
  s += "Tile Type Clicked: " + tile_type_str + "\n";
  s += "Tile Light Level: " + tile_light_level_str + "\n";
  s += "\n";

  DrawRectangleLines(x, y, w, h, GRAY);
  DrawTextEx(get_global_font(), s.c_str(),
             (Vector2){(float)x + 10, (float)y + 10}, fontsize, 0.5f, WHITE);
}

inline void GameplayScene::draw_message_log() {
  const int max_messages = 12;
  const int pad = 10;
  const int w = 500;
  const int h = 500;
  const float x = GetScreenWidth() - w - 10;
  const int y = h + pad + pad + pad;
  // const int fontsize = 20;
  const int fontsize = get_global_font().baseSize;
  const Color c0 = Fade(BLACK, 0.5f);
  DrawRectangle(x, y, w, h, c0);
  string s = "Messages:\n\n";
  // for (auto e : dungeon_events) {
  int count = 0;
  for (int i = (int)dungeon_events.size() - 1; i >= 0 && count < max_messages;
       i--) {
    DungeonEvent &e = dungeon_events[i];
    s += get_dungeon_event_str(e);
    count++;
  }
  DrawRectangleLines(x, y, w, h, GRAY);
  DrawTextEx(get_global_font(), s.c_str(),
             (Vector2){(float)x + 10, (float)y + 10}, fontsize, 0.5f, WHITE);
}

const string
GameplayScene::get_dungeon_event_str(const DungeonEvent &dungeon_event) {

  string s = "";
  switch (dungeon_event.get_type()) {
  case EVENT_ENTITY_MOVE_SUCCESS:
    s = "Moved to ";
    break;
  case EVENT_ENTITY_MOVE_FAIL:
    s = "Cannot move to ";
    break;
  default:
    s = "Unknown action";
    break;
  }
  s += "\n";
  return s;
}

void GameplayScene::cleanup() {
  for (int i = 0; i < (int)get_entity_ids().size(); i++) {
    entity_id id = get_entity_ids()[i];
    if (get_sprites()[id]->get_is_marked_for_deletion()) {
      get_sprites().erase(id);
      // also need to erase from entity_ids
      get_entity_ids().erase(get_entity_ids().begin() + i);
      // update the index so that we don't skip any
      i--;
    }
  }
}

inline void GameplayScene::draw() {
  BeginMode2D(get_camera2d());
  Color clear_color = BLACK;
  ClearBackground(clear_color);
  //   draw all tiles first
  //   entities existing on tiles will get drawn
  for (int i = 0; i < dungeon_floor.get_gridsize(); i++) {
    for (int j = 0; j < dungeon_floor.get_gridsize(); j++) {
      const tile_type t = dungeon_floor.get_tile_type(i, j);
      const string s = tile_key_for_type(t);
      draw_tile(s, i, j);
    }
  }

  EndMode2D();

  if (get_hud_on()) {
    draw_hud();
  }

  if (get_debug_panel_on()) {
    DrawFPS(10, GetScreenHeight() - 50);
    draw_debug_panel();
  }

  if (display_message_log) {
    draw_message_log();
  }

  draw_controls();
  handle_popup_manager();
  incr_current_frame();
}

const string GameplayScene::tile_key_for_type(const tile_type t) const {
  string tile_key = "tile-";
  switch (t) {
  case TILE_FLOOR_BASIC:
    tile_key += "stone";
    break;
  case TILE_FLOOR_STONE:
    tile_key += "stone";
    break;
  case TILE_FLOOR_WOOD:
    tile_key += "wood";
    break;
  case TILE_FLOOR_DIRT:
    tile_key += "dirt";
    break;
  case TILE_FLOOR_UPSTAIRS:
    tile_key += "upstairs";
    break;
  case TILE_FLOOR_DOWNSTAIRS:
    tile_key += "downstairs";
    break;
  default:
    tile_key += "void";
    break;
  }
  return tile_key;
}

inline void GameplayScene::draw_tile(const string tile_key, const int i,
                                     const int j) {
  shared_ptr<texture_info> t = get_texture_info(tile_key);
  const int t_sz = 20;
  const float scale = get_global_scale();
  Rectangle src = {0, 0, (float)t->texture.width, (float)t->texture.height};
  Rectangle dest = {i * t_sz * scale,
                    j * t_sz * scale -
                        (t->texture.height * scale - t_sz * scale),
                    t->texture.width * scale, t->texture.height * scale};
  Vector2 origin = {0, 0};
  Color color = WHITE;

  Tile &tile = dungeon_floor.get_tile_by_col_row(i, j);
  const float light_incr = 1.0f / tile.get_max_light_level();
  float alpha = 1.0f - tile.get_light_level() * light_incr;

  DrawTexturePro(t->texture, src, dest, origin, 0.0f, color);

  // now, for an upgrade, we want to draw the entities on the tile before we
  // draw the shade box this way, the entities will be drawn below the shade box

  // draw the entities on the tile
  const vector<entity_id> &entities = dungeon_floor.get_entities(i, j);
  // int width = t_sz;
  // int height = t_sz;
  //  to properly shade the entities, we will have to find the tallest
  //  we will make an effort to keep entity sprites within the tile width
  //  but some sprites may be taller and in fact are at times
  //  Draw a black rectangle in front of the tile
  DrawRectangle(dest.x, dest.y, dest.width, dest.height, Fade(BLACK, alpha));

  for (auto &e : entities) {
    // get the sprite's dimensions
    // if (get_sprite(e)->get_height() / scale > height) {
    //  height = get_sprite(e)->get_height() / scale;
    //}
    get_sprite(e)->draw();
  }
  // update the dest.height
  // dest.height = height * scale;
  // update the dest.y
  // dest.y = j * t_sz * scale - (height * scale - t_sz * scale);

  //  check to see if we need to 'select' the tile
  handle_tile_click(dest, i, j);
}

inline void GameplayScene::handle_tile_click(const Rectangle dest, const int i,
                                             const int j) {
  if (tile_is_selected) {
    if (i == last_tile_click_pos.x && j == last_tile_click_pos.y) {
      DrawRectangleLinesEx(dest, 4, GREEN);
    } else if (CheckCollisionPointRec(last_mouse_click_pos, dest)) {
      last_tile_click_pos = (Vector2){(float)i, (float)j};
      // increase the light level of the tile
    }
  }
}

inline void GameplayScene::handle_popup_manager() {
  if (show_test_popup) {
    if (get_popup_manager() != nullptr) {
      const Vector2 dest_vector =
          (Vector2){get_sprite(player_id)->get_dest().x - 20,
                    get_sprite(player_id)->get_dest().y - 40};
      Vector2 s = GetWorldToScreen2D(dest_vector, get_camera2d());
      // Get the screen space position for
      // a 2d camera world space position
      get_popup_manager()->draw(s.x, s.y);
    }
  }
}

void GameplayScene::close() {
  mPrint("Closing gameplay scene...");
  mPrint("Unloading textures...");
  for (auto &t : get_textures()) {
    UnloadTexture(t.second->texture);
  }
  mPrint("Clearing textures...");
  get_textures().clear();
  mPrint("Clearing sprites...");
  get_sprites().clear();
  mPrint("Clearing entity ids...");
  get_entity_ids().clear();
  mPrint("Unloading font...");
  UnloadFont(get_global_font());
  // if (music != NULL) {
  //  stop music
  //  Mix_PauseMusic();
  //  Mix_FreeMusic(music);
  //}
  set_has_been_initialized(false);
  player_id = -1;
  get_popup_manager()->zero_alpha();
  mPrint("Scene closed.");
}
