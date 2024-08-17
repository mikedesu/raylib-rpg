#include "GameplayScene.h"
// #include "Sprite.h"
#include "Tile.h"
#include "mPrint.h"
#include "raymath.h"
#include <string>

using std::string;
using std::to_string;

GameplayScene::GameplayScene() {
  mPrint("GameplayScene constructor");
  set_control_mode(CONTROL_MODE_PLAYER);
  set_texture_filepath("game_textures.txt");
  // set_global_scale(1.0f);
  //  set_global_scale(2.0f);
  set_global_scale(3.0f);
  //  set_global_scale(4.0f);
  set_scene_transition(SCENE_TRANSITION_IN);
  set_scene_type(SCENE_TYPE_GAMEPLAY);
  // load_music("/home/darkmage/Music/darkmage/lets-fkn-go.mp3");
}

GameplayScene::~GameplayScene() { mPrint("GameplayScene destructor"); }

void GameplayScene::gameover() { set_scene_transition(SCENE_TRANSITION_OUT); }

void GameplayScene::update_player_movement() {}

void GameplayScene::update_enemy_movement() {}

void GameplayScene::handle_player_collision() {}

void GameplayScene::update() {
  /*
   * we will need to update the relative onscreen positions of the sprites
   * depending on
   * 1. position in the dungeon
   * 2. global scale
   */
  for (auto &s : get_sprites()) {
    Vector2 dungeon_pos = dungeon_floor.get_entity_position(s.first);
    s.second->update(dungeon_pos);
  }

  if (player_did_move) {
    turn_count++;
    player_did_move = false;
  }
}

void GameplayScene::handle_camera_input() {
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

  const float zoom_incr = 1;
  if (IsKeyDown(KEY_RIGHT_SHIFT) && IsKeyPressed(KEY_Z)) {
    // mPrint("right shift + z");
    set_scale(get_global_scale() - zoom_incr);
  } else if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_Z)) {
    // mPrint("left shift + z");
    set_scale(get_global_scale() - zoom_incr);
  } else if (IsKeyPressed(KEY_Z)) {
    // mPrint("z");
    set_scale(get_global_scale() + zoom_incr);
  }
}

void GameplayScene::handle_dungeon_move_pos(const entity_id id,
                                            const Vector2 c_pos,
                                            const Vector2 t_pos) {
  if (Vector2Equals(c_pos, t_pos)) {
    return;
  }

  tile_type t = dungeon_floor.get_tile_type(t_pos.x, t_pos.y);

  // mPrint("Tile type: " + to_string(t));

  // message_log.push_back(to_string(id) + " moving to " + to_string(t_pos.x) +
  //                       ", " + to_string(t_pos.y));

  switch (t) {
  case TILE_FLOOR_BASIC:
  case TILE_FLOOR_STONE:
  case TILE_FLOOR_WOOD:
  case TILE_FLOOR_DIRT:
    // get_sprites()[id]->set_dungeon_position(t_pos);
    // dungeon_floor.set_entity_position(id, t_pos);
    {
      bool r = dungeon_floor.move_entity_to_tile(id, t_pos);
      if (r) {
        const string s = "Moved to " + to_string((int)t_pos.x) + ", " +
                         to_string((int)t_pos.y);
        message_log.push_back(s);
        get_popup_manager()->render("Moved");
      } else {
        message_log.push_back("Cannot move to " + to_string((int)t_pos.x) +
                              ", " + to_string((int)t_pos.y));
        get_popup_manager()->render("Cannot move");
      }
    }

    // message_log.push_back(to_string(id) + " moved to " +
    //                       to_string((int)t_pos.x) + ", " +
    //                       to_string((int)t_pos.y));
    break;
  default:
    message_log.push_back("Bumped into wall or other");
    get_popup_manager()->render("Cannot move");
    // message_log.push_back(to_string(id) + " cannot move to " +
    //                       to_string((int)t_pos.x) + ", " +
    //                       to_string((int)t_pos.y));
    break;
  }
}

void GameplayScene::handle_dungeon_move_dir(const entity_id id,
                                            const Vector2 direction) {
  // we have to check the dungeon position of the sprite
  // if the target tile is a location we cant move to, like none, void, or wall,
  // then we don't move

  // get the current dungeon position of the sprite
  // Vector2 cur_pos = get_sprites()[id]->get_dungeon_position();
  Vector2 cur_pos = dungeon_floor.get_entity_position(id);

  Vector2 t_pos = Vector2Add(cur_pos, direction);

  // if the locations are equal, no move is executed
  // this way be interpeted as a "wait" action in the future
  handle_dungeon_move_pos(id, cur_pos, t_pos);

  // check if the target dungeon position is valid
  // if (t == TILE_FLOOR_BASIC) {
  // move the sprite to the target position
  //}
}

void GameplayScene::handle_player_input() {
  if (IsKeyPressed(KEY_UP)) {
    handle_dungeon_move_dir(player_id, (Vector2){0, -1});
    player_did_move = true;
  }
  if (IsKeyPressed(KEY_DOWN)) {
    handle_dungeon_move_dir(player_id, (Vector2){0, 1});
    player_did_move = true;
  }
  if (IsKeyPressed(KEY_LEFT)) {
    handle_dungeon_move_dir(player_id, (Vector2){-1, 0});
    player_did_move = true;
  }
  if (IsKeyPressed(KEY_RIGHT)) {
    handle_dungeon_move_dir(player_id, (Vector2){1, 0});
    player_did_move = true;
  }

  // diagonals and numpad keypad entry
  if (IsKeyPressed(KEY_KP_7)) {
    handle_dungeon_move_dir(player_id, (Vector2){-1, -1});
    player_did_move = true;
  }

  if (IsKeyPressed(KEY_KP_9)) {
    handle_dungeon_move_dir(player_id, (Vector2){1, -1});
    player_did_move = true;
  }

  if (IsKeyPressed(KEY_KP_1)) {
    handle_dungeon_move_dir(player_id, (Vector2){-1, 1});
    player_did_move = true;
  }

  if (IsKeyPressed(KEY_KP_3)) {
    handle_dungeon_move_dir(player_id, (Vector2){1, 1});
    player_did_move = true;
  }

  if (IsKeyPressed(KEY_KP_8)) {
    handle_dungeon_move_dir(player_id, (Vector2){0, -1});
    player_did_move = true;
  }

  if (IsKeyPressed(KEY_KP_2)) {
    handle_dungeon_move_dir(player_id, (Vector2){0, 1});
    player_did_move = true;
  }

  if (IsKeyPressed(KEY_KP_4)) {
    handle_dungeon_move_dir(player_id, (Vector2){-1, 0});
    player_did_move = true;
  }

  if (IsKeyPressed(KEY_KP_6)) {
    handle_dungeon_move_dir(player_id, (Vector2){1, 0});
    player_did_move = true;
  }

  if (IsKeyPressed(KEY_R)) {
    // change tile at 0, 0 to floor
    // dungeon_floor.set_tile_type(0, 0, TILE_FLOOR_BASIC);

    // spawn a torch at a random location
    spawn_torch(
        (Vector2){(float)GetRandomValue(0, dungeon_floor.get_gridsize() - 1),
                  (float)GetRandomValue(0, dungeon_floor.get_gridsize() - 1)});
  }

  if (IsKeyPressed(KEY_P)) {
    // popup message
    show_test_popup = !show_test_popup;

    // if (show_test_popup) {
    //   get_popup_manager()->render("Test Popup");
    // }
  }

  // if (IsKeyPressed(KEY_T)) {
  //
  //  }
}

void GameplayScene::handle_input() {
  if (IsKeyPressed(KEY_D)) {
    flip_debug_panel();
  }

  if (IsKeyPressed(KEY_F)) {
    ToggleFullscreen();
  }

  if (IsKeyPressed(KEY_C)) {
    if (get_control_mode() == CONTROL_MODE_PLAYER) {
      set_control_mode(CONTROL_MODE_CAMERA);
    } else {
      set_control_mode(CONTROL_MODE_PLAYER);
    }
  }

  if (get_control_mode() == CONTROL_MODE_CAMERA) {
    handle_camera_input();
  } else if (get_control_mode() == CONTROL_MODE_PLAYER) {
    handle_player_input();
  }
}

void GameplayScene::set_scale(const float f) {
  // assert(s > 0.0f);
  if (f > 0) {
    set_global_scale(f);
    for (auto &s : get_sprites()) {
      s.second->set_scale(f);
    }
  }
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
    mPrint("Spawning player...");
    // spawning a player is a function of spawn_entity
    // we can write code to put into a function that spawns the player

    // const int offset_x = 0;
    // const int offset_y = 0;

    spawn_player((Vector2){1, 1});

    // set the player's dungeon position
    // get_sprites()[player_id]->set_dungeon_position((Vector2){1, 1});

    // for (int i = 0; i < dungeon_floor.get_gridsize(); i++) {
    //   for (int j = 0; j < dungeon_floor.get_gridsize(); j++) {
    //     if (dungeon_floor.get_tile_type(i, j) == TILE_FLOOR_BASIC) {
    //       spawn_tile_stone(i, j, i * 20, j * 20);
    //     } else if (dungeon_floor.get_tile_type(i, j) == TILE_WALL_BASIC) {
    //       spawn_tile_void(i * 20, j * 20);
    //     }
    //   }
    // }

    // spawn_goblin((Vector2){2, 2});

    mPrint("Setting camera offset...");
    get_camera2d().target.x = -450;
    get_camera2d().target.y = -220;

    mPrint("Loading sound effects...");

    set_has_been_initialized(true);

    mPrint("GameplayScene initialized");
  }
  return true;
}

const entity_id GameplayScene::spawn_player(const Vector2 pos) {
  entity_id id = spawn_entity("player", 0, 0, SPRITETYPE_PLAYER, true);
  player_id = id;
  dungeon_floor.set_entity_position(id, pos);
  return id;
}

const entity_id GameplayScene::spawn_goblin(const Vector2 pos) {
  entity_id id = spawn_entity("goblin", 0, 0, SPRITETYPE_ENEMY, true);
  dungeon_floor.set_entity_position(id, pos);
  return id;
}

const entity_id GameplayScene::spawn_torch(const Vector2 pos) {
  entity_id id = spawn_entity("torch", 0, 0, SPRITETYPE_ITEM, true);
  dungeon_floor.set_entity_position(id, pos);
  return id;
}

// entity_id GameplayScene::spawn_tile_stone(float i, float j, float x, float y)
// {
//   entity_id id = spawn_entity("tile-stone", x, y, SPRITETYPE_TILE, false);
//   // set the dungeon position of the sprite
//   get_sprites()[id]->set_dungeon_position((Vector2){i, j});
//   // player_id = id;
//   return id;
// }

// entity_id GameplayScene::spawn_tile_void(float x, float y) {
//   entity_id id = spawn_entity("tile-void", x, y, SPRITETYPE_TILE, false);
//   player_id = id;
//   return id;
// }

void GameplayScene::draw_debug_panel() {
  string camera_info_str =
      "Current Frame: " + to_string(get_current_frame()) + "\n" +
      "Control mode: " + to_string(get_control_mode()) + "\n" +
      "Camera target: " + to_string(get_camera2d().target.x) + ", " +
      to_string(get_camera2d().target.y) + "\n" + "GameplayScene" +
      "Sprites: " + to_string(get_sprites().size()) + "\n" +
      "IsPaused: " + to_string(get_paused()) + "\n" +
      "Global Scale: " + to_string(get_global_scale()) + "\n";
  DrawRectangle(0, 0, 500, 200, Fade(BLACK, 0.5f));
  DrawTextEx(get_global_font(), camera_info_str.c_str(), (Vector2){10, 10}, 16,
             0.5f, WHITE);
}

inline void GameplayScene::draw_controls() {
  const int fontsize = 24;
  const int x = 10;
  const int y = 10;
  const string s = "Controls: \n"
                   "Arrow keys: move player\n"
                   "Numpad keys: move player\n"
                   "R: spawn torch\n"
                   "D: toggle debug panel\n"
                   "C: toggle control mode\n"
                   "Zz: zoom in/out\n"
                   "T: test popup\n"
                   "P: toggle pause\n"
                   "Q: quit\n";
  DrawText(s.c_str(), x, y, fontsize, WHITE);
}

inline void GameplayScene::draw_hud() {
  // draw a black box on the right side of the screen
  const int w = 500;
  const int h = GetScreenHeight();
  const float x = GetScreenWidth() - w;
  const int y = 0;
  const int fontsize = 24;
  const int max_messages = 30;

  DrawRectangle(x, y, w, h, BLACK);
  // draw some text
  const string s =
      "Player Position: " +
      to_string((int)dungeon_floor.get_entity_position(player_id).x) + ", " +
      to_string((int)dungeon_floor.get_entity_position(player_id).y) + "\n" +
      "Camera: " + to_string((int)get_camera2d().target.x) + ", " +
      to_string((int)get_camera2d().target.y) + "\n" +
      "Turn: " + to_string(turn_count) + "\n\n";
  // "Message Log: \n";

  string messages = "";
  // iterate backwards thru message_log and construct a big string
  // only show the last 10 messages
  int count = 0;
  for (int i = (int)message_log.size() - 1; i >= 0 && count < max_messages;
       i--) {
    messages += message_log[i] + "\n";
    count++;
  }

  const string s2 = s + messages;

  // DrawTextEx(get_global_font(), s2.c_str(), (Vector2){x + 10, y + 10},
  // fontsize,
  //            0.5f, WHITE);

  DrawText(s2.c_str(), x + 10, y + 10, fontsize, WHITE);
}

void GameplayScene::cleanup() {
  // mPrint("cleanup");
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
  // Color clear_color = (Color){0x10, 0x10, 0x10, 0xFF};
  Color clear_color = BLACK;
  ClearBackground(clear_color);
  // const int unit = 20;
  //  const int scaled_unit = unit * get_global_scale();
  //   draw all tiles first

  for (int i = 0; i < dungeon_floor.get_gridsize(); i++) {
    for (int j = 0; j < dungeon_floor.get_gridsize(); j++) {
      // if (dungeon_floor.get_tile_type(i, j) == TILE_FLOOR_BASIC) {
      //   draw_tile("tile-stone", i, j);
      // } else if (dungeon_floor.get_tile_type(i, j) == TILE_WALL_BASIC) {
      //   draw_tile("tile-void", i, j);
      // }

      const tile_type t = dungeon_floor.get_tile_type(i, j);
      const string s = tile_key_for_type(t);
      draw_tile(s, i, j);
    }
  }

  // draw all other sprites
  for (auto &s : get_sprites()) {
    s.second->draw();
  }

  EndMode2D();
  handle_draw_debug_panel();
  draw_controls();

  handle_popup_manager();
  incr_current_frame();
}

const string GameplayScene::tile_key_for_type(const tile_type t) {
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

  case TILE_WALL_BASIC:
    tile_key += "void";
    break;
  default:
    tile_key += "void";
    break;
  }
  return tile_key;
}

inline void GameplayScene::draw_tile(const string tile_key, const int i,
                                     const int j) {
  const int x = i * 20 * get_global_scale();
  const int y = j * 20 * get_global_scale();
  texture_info &t = get_textures()[tile_key];
  Rectangle src = {0.0f, 0.0f, (float)t.texture.width, (float)t.texture.height};
  Rectangle dest = {(float)x, (float)y,
                    (float)t.texture.width * get_global_scale(),
                    (float)t.texture.height * get_global_scale()};
  Vector2 origin = {0, 0};
  Color color = WHITE;
  DrawTexturePro(t.texture, src, dest, origin, 0.0f, color);
}

inline void GameplayScene::handle_popup_manager() {
  if (show_test_popup) {
    if (get_popup_manager() != nullptr) {

      // const float x = GetScreenWidth() / 2.0f - 100.0f - 50.0f;
      // const float y = GetScreenHeight() / 2.0f - 100.0f - 50.0f;

      const Rectangle dest = get_sprite(player_id)->get_dest();
      const int off_x = -20;
      const int off_y = -40;
      const Vector2 dest_vector = (Vector2){dest.x + off_x, dest.y + off_y};

      Vector2 s = GetWorldToScreen2D(dest_vector, get_camera2d());
      // Get the screen space position for
      // a 2d camera world space position
      get_popup_manager()->draw(s.x, s.y);
    }
  }
}

inline void GameplayScene::handle_draw_debug_panel() {

  if (get_hud_on()) {
    draw_hud();
  }

  if (get_debug_panel_on()) {
    DrawFPS(GetScreenWidth() - 80, 10);
    draw_debug_panel();
  }
}

void GameplayScene::close() {
  mPrint("Closing gameplay scene...");
  mPrint("Unloading textures...");
  for (auto &t : get_textures()) {
    UnloadTexture(t.second.texture);
  }
  mPrint("Clearing textures...");
  get_textures().clear();
  mPrint("Clearing sprites...");
  get_sprites().clear();
  get_bgsprites().clear();
  mPrint("Clearing entity ids...");
  get_entity_ids().clear();
  get_bg_entity_ids().clear();
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
