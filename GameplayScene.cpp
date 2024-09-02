#include "DungeonFloor.h"
#include "Entity.h"
#include "EntityType.h"
#include "GameplayScene.h"
#include "mPrint.h"
#include "raymath.h"
#include <cstring>

static EntityId next_entity_id = 0;

void GameplayScene_create(GameplayScene& g) {
    mPrint("GameplayScene constructor");
    g.last_mouse_click_pos = (Vector2){0, 0};
    g.last_tile_click_pos = (Vector2){-1, -1};
    g.controlmode = CONTROL_MODE_PLAYER;

    g.texture_filepath = "game_textures.txt";
    g.global_scale = 3.0f;
    g.transition = SCENE_TRANSITION_IN;
    g.scenetype = SCENE_TYPE_GAMEPLAY;

    // initialize the dungeonfloor
    //DungeonFloor_create(g.dungeon_floor, 20, 20);
    DungeonFloor_create(g.dungeon_floor);
}

void GameplayScene_destroy(GameplayScene& g) {
    mPrint("GameplayScene destructor");
}

void GameplayScene_gameover(GameplayScene& g) {
    //GameplayScene_set_scene_transition(g, SCENE_TRANSITION_OUT);
}

void GameplayScene_update(GameplayScene& g) {
    // we will need to update the relative onscreen positions of the sprites
    // depending on
    // 1. position in the dungeon
    // 2. global scale
    for(auto& s : g.sprites) {
        //Vector2 dungeon_pos = g.dungeon_floor.get_entity_position(s.first);
        Vector2 dungeon_pos = DungeonFloor_get_entity_position(g.dungeon_floor, s.first);
        //s.second->update(dungeon_pos);
        Sprite_update(s.second, dungeon_pos);
    }
    if(g.player_attempted_move) {
        g.turn_count++;
        g.player_attempted_move = false;
    }
}

inline void GameplayScene_handle_camera_input_move(GameplayScene& g) {
    const int cam_move = 10;
    if(IsKeyDown(KEY_UP)) {
        g.camera2d.target.y -= cam_move;
    }
    if(IsKeyDown(KEY_DOWN)) {
        g.camera2d.target.y += cam_move;
    }
    if(IsKeyDown(KEY_LEFT)) {
        g.camera2d.target.x -= cam_move;
    }
    if(IsKeyDown(KEY_RIGHT)) {
        g.camera2d.target.x += cam_move;
    }
}

inline void GameplayScene_handle_camera_input_zoom(GameplayScene& g) {
    const float zoom_incr = 1;
    if(IsKeyDown(KEY_RIGHT_SHIFT) && IsKeyPressed(KEY_Z)) {
        GameplayScene_set_scale(g, g.global_scale - zoom_incr);
        GameplayScene_center_camera_on_player(g);
        GameplayScene_update_tile_click(g);
    } else if(IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_Z)) {
        GameplayScene_set_scale(g, g.global_scale - zoom_incr);
        GameplayScene_center_camera_on_player(g);
        GameplayScene_update_tile_click(g);
    } else if(IsKeyPressed(KEY_Z)) {
        GameplayScene_set_scale(g, g.global_scale + zoom_incr);
        GameplayScene_center_camera_on_player(g);
        GameplayScene_update_tile_click(g);
    }
}

void GameplayScene_update_tile_click(GameplayScene& g) {
    g.prev_tile_click_zoom_level = g.tile_click_zoom_level;
    g.tile_click_zoom_level = g.global_scale;
    g.last_mouse_click_pos = (Vector2){-1, -1};
}

void GameplayScene_center_camera_on_player(GameplayScene& g) {
    //Rectangle pos = g.sprites[g.player_id]->get_dest();
    Rectangle pos = Sprite_get_dest(g.sprites[g.player_id]);
    // the update isnt smooth
    // we need to try and smooth it out
    const int targetx = pos.x - (GetScreenWidth() / 2.0f);
    const int targety = pos.y - (GetScreenHeight() / 2.0f);
    const int cx = g.camera2d.target.x;
    const int cy = g.camera2d.target.y;
    const int dx = targetx - cx;
    const int dy = targety - cy;
    g.camera2d.target.x += dx;
    g.camera2d.target.y += dy;
    //g.camera2d.target.x = pos.x - (GetScreenWidth() / 2.0f);
    //g.camera2d.target.y = pos.y - (GetScreenHeight() / 2.0f);
}

inline void GameplayScene_handle_camera_input(GameplayScene& g) {
    GameplayScene_handle_camera_input_move(g);
    GameplayScene_handle_camera_input_zoom(g);
}

inline void GameplayScene_handle_tile_selection_input(GameplayScene& g) {
    if(IsKeyPressed(KEY_ESCAPE)) {
        g.tile_is_selected = false;
        g.last_tile_click_pos = (Vector2){-1, -1};
    }

    if(IsKeyPressed(KEY_ENTER)) {
        g.tile_is_selected = false;
        g.last_tile_click_pos = (Vector2){-1, -1};
    }

    if(IsKeyPressed(KEY_UP)) {
        if(g.last_tile_click_pos.y > 0) {
            g.last_tile_click_pos.y--;
            g.tile_is_selected = true;
        }
    }

    if(IsKeyPressed(KEY_DOWN)) {
        //if(g.last_tile_click_pos.y < g.dungeon_floor.get_gridsize() - 1) {
        if(g.last_tile_click_pos.y < DungeonFloor_get_gridsize(g.dungeon_floor) - 1) {
            g.last_tile_click_pos.y++;
            g.tile_is_selected = true;
        }
    }

    if(IsKeyPressed(KEY_LEFT)) {
        if(g.last_tile_click_pos.x > 0) {
            g.last_tile_click_pos.x--;
            g.tile_is_selected = true;
        }
    }

    if(IsKeyPressed(KEY_RIGHT)) {
        //if(g.last_tile_click_pos.x < g.dungeon_floor.get_gridsize() - 1) {
        if(g.last_tile_click_pos.x < DungeonFloor_get_gridsize(g.dungeon_floor) - 1) {
            g.last_tile_click_pos.x++;
            g.tile_is_selected = true;
        }
    }

    if(IsKeyPressed(KEY_R)) {
        GameplayScene_spawn_torch(g, g.last_tile_click_pos);
    }

    if(IsKeyPressed(KEY_T)) {
        GameplayScene_remove_torch(g, g.last_tile_click_pos);
    }
}

bool GameplayScene_handle_dungeon_move_pos(GameplayScene& g,
                                           const EntityId id,
                                           const Vector2 c_pos,
                                           const Vector2 t_pos) {
    bool retval = false;
    //  if they are different tiles...
    if(!Vector2Equals(c_pos, t_pos)) {
        //TileType t = g.dungeon_floor.get_tile_type(t_pos.x, t_pos.y);
        TileType t = DungeonFloor_get_tile_type(g.dungeon_floor, t_pos.x, t_pos.y);
        switch(t) {
        case TILE_FLOOR_BASIC:
        case TILE_FLOOR_STONE:
        case TILE_FLOOR_WOOD:
        case TILE_FLOOR_DIRT:
        case TILE_FLOOR_UPSTAIRS:
        case TILE_FLOOR_DOWNSTAIRS: {
            //bool r = g.dungeon_floor.move_entity_to_tile(id, t_pos);
            bool r = DungeonFloor_move_entity_to_tile(g.dungeon_floor, id, t_pos);
            if(r) {
                g.dungeon_events.push_back(DungeonEvent(id, EVENT_ENTITY_MOVE_SUCCESS, t_pos));

                // GameplayScene_get_popup_manager(g)->render("Moved");
                //g.popup_manager->render("Moved");
                PopupManager_render(g.popup_manager, "Moved");

                retval = true;
            } else {
                g.dungeon_events.push_back(DungeonEvent(id, EVENT_ENTITY_MOVE_FAIL, t_pos));
                //g.popup_manager->render("Cannot move");
                PopupManager_render(g.popup_manager, "Cannot move");
                retval = false;
            }
        } break;
        default:
            g.dungeon_events.push_back(DungeonEvent(id, EVENT_ENTITY_MOVE_FAIL, t_pos));
            //g.popup_manager->render("Cannot move");
            PopupManager_render(g.popup_manager, "Cannot move");
            retval = false;
            break;
        }
        // else its a wait
    } else {
        g.dungeon_events.push_back(DungeonEvent(id, EVENT_ENTITY_MOVE_WAIT, t_pos));
        //g.popup_manager->render("Wait");
        PopupManager_render(g.popup_manager, "Wait");
        retval = true;
    }
    return retval;
}

Vector2 GameplayScene_handle_dungeon_move_dir(GameplayScene& g,
                                              const EntityId id,
                                              const Vector2 direction) {
    // we have to check the dungeon position of the sprite
    // if the target tile is a location we cant move to, like none, void, or wall,
    // then we don't move
    // get the current dungeon position of the sprite
    //Vector2 cur_pos = g.dungeon_floor.get_entity_position(id);
    Vector2 cur_pos = DungeonFloor_get_entity_position(g.dungeon_floor, id);
    Vector2 t_pos = Vector2Add(cur_pos, direction);
    // if the locations are equal, no move is executed
    // this way be interpeted as a "wait" action in the future
    bool success = GameplayScene_handle_dungeon_move_pos(g, id, cur_pos, t_pos);
    Vector2 retval = (Vector2){0, 0};
    if(success) {
        retval = direction;
    }
    return retval;
}

inline void GameplayScene_handle_player_move_direction(GameplayScene& g) {
    const int tilesize = 20;
    Vector2 move_dir = (Vector2){0, 0};
    if(IsKeyPressed(KEY_UP)) {
        g.player_attempted_move = true;
        // set the player sprite's context
        move_dir = GameplayScene_handle_dungeon_move_dir(g, g.player_id, (Vector2){0, -1});
        //g.sprites[g.player_id]->set_context(1);
        Sprite_set_context(g.sprites[g.player_id], 1);
    } else if(IsKeyPressed(KEY_DOWN)) {
        g.player_attempted_move = true;
        move_dir = GameplayScene_handle_dungeon_move_dir(g, g.player_id, (Vector2){0, 1});
        //g.sprites[g.player_id]->set_context(1);
        Sprite_set_context(g.sprites[g.player_id], 1);
    } else if(IsKeyPressed(KEY_LEFT)) {
        g.player_attempted_move = true;
        move_dir = GameplayScene_handle_dungeon_move_dir(g, g.player_id, (Vector2){-1, 0});
        //g.sprites[g.player_id]->set_context(1);
        Sprite_set_context(g.sprites[g.player_id], 1);
    } else if(IsKeyPressed(KEY_RIGHT)) {
        g.player_attempted_move = true;
        move_dir = GameplayScene_handle_dungeon_move_dir(g, g.player_id, (Vector2){1, 0});
        //g.sprites[g.player_id]->set_context(1);
        Sprite_set_context(g.sprites[g.player_id], 1);
    } else if(IsKeyPressed(KEY_PERIOD)) {
        g.player_attempted_move = true;
        move_dir = GameplayScene_handle_dungeon_move_dir(g, g.player_id, (Vector2){0, 0});
        //g.sprites[g.player_id]->set_context(1);
        Sprite_set_context(g.sprites[g.player_id], 1);
    }
    //  update the camera
    //GameplayScene_center_camera_on_player(g);
    g.camera2d.target.x += move_dir.x * tilesize * g.global_scale;
    g.camera2d.target.y += move_dir.y * tilesize * g.global_scale;
}

void GameplayScene_remove_torch(GameplayScene& g, const Vector2 pos) {
    if(Vector2Equals(pos, (Vector2){-1, -1}))
        return;
    //const vector<EntityId>& entities = g.dungeon_floor.get_entities(pos.x, pos.y);
    const vector<EntityId>& entities = DungeonFloor_get_entities(g.dungeon_floor, pos.x, pos.y);
    for(auto entity_id : entities) {
        //if(g.dungeon_floor.get_entity_type(entity_id) == ENTITY_TORCH) {
        if(DungeonFloor_get_entity_type(g.dungeon_floor, entity_id) == ENTITY_TORCH) {
            //g.dungeon_floor.remove_entity(entity_id);
            DungeonFloor_remove_entity(g.dungeon_floor, entity_id);
            GameplayScene_decrease_lighting_at(g, pos, 3, 3);
            // decrease_lighting_at(pos, 2);
            break;
        }
    }
}

inline void GameplayScene_handle_player_input(GameplayScene& g) {
    //handle_player_mouse_click();
    if(IsKeyPressed(KEY_R)) {
        //GameplayScene_spawn_torch(g, g.dungeon_floor.get_entity_position(g.player_id));
        GameplayScene_spawn_torch(g,
                                  DungeonFloor_get_entity_position(g.dungeon_floor, g.player_id));
    }

    if(IsKeyPressed(KEY_T)) {
        //GameplayScene_remove_torch(g, g.dungeon_floor.get_entity_position(g.player_id));
        GameplayScene_remove_torch(g,
                                   DungeonFloor_get_entity_position(g.dungeon_floor, g.player_id));
    }

    GameplayScene_handle_player_move_direction(g);
    g.prev_tile_click_zoom_level = g.tile_click_zoom_level;
}

void GameplayScene_increase_lighting_at(GameplayScene& g,
                                        const Vector2 loc,
                                        const int light_level,
                                        const int dist) {
    //if(loc.x < 0 || loc.y < 0 || loc.x >= g.dungeon_floor.get_gridsize() ||
    if(loc.x < 0 || loc.y < 0 || loc.x >= DungeonFloor_get_gridsize(g.dungeon_floor) ||
       loc.y >= DungeonFloor_get_gridsize(g.dungeon_floor)) {
        return;
    }

    if(light_level <= 0) {
        return;
    }

    if(dist <= 0) {
        return;
    }

    if(light_level < dist) {
        return;
    }

    //const int gridsize = g.dungeon_floor.get_gridsize();
    const int gridsize = DungeonFloor_get_gridsize(g.dungeon_floor);
    for(int j = 0; j < dist; j++) {
        const int x0 = loc.x - j;
        const int x1 = loc.x + j;
        for(int i = 0; i < dist - j; i++) {
            const int light_incr = light_level - j - i;
            const int y0 = loc.y - i;
            const int y1 = loc.y + i;
            if(x0 >= 0 && x0 < gridsize && y0 >= 0 && y0 < gridsize) {
                //g.dungeon_floor.get_tile_by_col_row(x0, y0).increase_light_level_by(light_incr);

                //Tile_increase_light_level_by(g.dungeon_floor.get_tile_by_col_row(x0, y0),
                Tile_increase_light_level_by(
                    DungeonFloor_get_tile_by_col_row(g.dungeon_floor, x0, y0), light_incr);
            }

            if(y0 != y1) {
                if(x0 >= 0 && x0 < gridsize && y1 >= 0 && y1 < gridsize) {
                    //g.dungeon_floor.get_tile_by_col_row(x0, y1).increase_light_level_by(light_incr);
                    //Tile_increase_light_level_by(g.dungeon_floor.get_tile_by_col_row(x0, y1),
                    Tile_increase_light_level_by(
                        DungeonFloor_get_tile_by_col_row(g.dungeon_floor, x0, y1), light_incr);
                }
            }
            if(x0 != x1) {
                if(x1 >= 0 && x1 < gridsize && y0 >= 0 && y0 < gridsize) {
                    //g.dungeon_floor.get_tile_by_col_row(x1, y0).increase_light_level_by(light_incr);

                    //Tile_increase_light_level_by(g.dungeon_floor.get_tile_by_col_row(x1, y0),
                    Tile_increase_light_level_by(
                        DungeonFloor_get_tile_by_col_row(g.dungeon_floor, x1, y0), light_incr);
                }
                if(y0 != y1) {
                    if(x1 >= 0 && x1 < gridsize && y1 >= 0 && y1 < gridsize) {
                        //g.dungeon_floor.get_tile_by_col_row(x1, y1).increase_light_level_by(light_incr);

                        //Tile_increase_light_level_by(g.dungeon_floor.get_tile_by_col_row(x1, y1),
                        Tile_increase_light_level_by(
                            DungeonFloor_get_tile_by_col_row(g.dungeon_floor, x1, y1), light_incr);
                    }
                }
            }
        }
    }
}

void GameplayScene_decrease_lighting_at(GameplayScene& g,
                                        const Vector2 loc,
                                        const int light_level,
                                        const int dist) {
    //if(loc.x < 0 || loc.y < 0 || loc.x >= g.dungeon_floor.get_gridsize() ||
    if(loc.x < 0 || loc.y < 0 || DungeonFloor_get_gridsize(g.dungeon_floor) ||
       //loc.y >= g.dungeon_floor.get_gridsize()) {
       loc.y >= DungeonFloor_get_gridsize(g.dungeon_floor)) {
        return;
    }

    if(light_level <= 0) {
        return;
    }

    if(dist <= 0) {
        return;
    }

    if(light_level < dist) {
        return;
    }

    //const int gridsize = g.dungeon_floor.get_gridsize();
    const int gridsize = DungeonFloor_get_gridsize(g.dungeon_floor);
    for(int j = 0; j < dist; j++) {
        const int x0 = loc.x - j;
        const int x1 = loc.x + j;
        for(int i = 0; i < dist - j; i++) {
            const int light_incr = light_level - j - i;
            const int y0 = loc.y - i;
            const int y1 = loc.y + i;
            if(x0 >= 0 && x0 < gridsize && y0 >= 0 && y0 < gridsize) {
                //g.dungeon_floor.get_tile_by_col_row(x0, y0).decrease_light_level_by(light_incr);

                //Tile_decrease_light_level_by(g.dungeon_floor.get_tile_by_col_row(x0, y0),
                Tile_decrease_light_level_by(
                    DungeonFloor_get_tile_by_col_row(g.dungeon_floor, x0, y0), light_incr);
            }

            if(y0 != y1) {
                if(x0 >= 0 && x0 < gridsize && y1 >= 0 && y1 < gridsize) {
                    //g.dungeon_floor.get_tile_by_col_row(x0, y1).decrease_light_level_by(light_incr);

                    //Tile_decrease_light_level_by(g.dungeon_floor.get_tile_by_col_row(x0, y1),
                    Tile_decrease_light_level_by(
                        DungeonFloor_get_tile_by_col_row(g.dungeon_floor, x0, y1), light_incr);
                }
            }
            if(x0 != x1) {
                if(x1 >= 0 && x1 < gridsize && y0 >= 0 && y0 < gridsize) {
                    //g.dungeon_floor.get_tile_by_col_row(x1, y0).decrease_light_level_by(light_incr);

                    //Tile_decrease_light_level_by(g.dungeon_floor.get_tile_by_col_row(x1, y0),
                    Tile_decrease_light_level_by(
                        DungeonFloor_get_tile_by_col_row(g.dungeon_floor, x1, y0), light_incr);
                }
                if(y0 != y1) {
                    if(x1 >= 0 && x1 < gridsize && y1 >= 0 && y1 < gridsize) {
                        //g.dungeon_floor.get_tile_by_col_row(x1, y1).decrease_light_level_by(
                        //    light_incr);

                        //Tile_decrease_light_level_by(g.dungeon_floor.get_tile_by_col_row(x1, y1),
                        Tile_decrease_light_level_by(
                            DungeonFloor_get_tile_by_col_row(g.dungeon_floor, x1, y1), light_incr);
                    }
                }
            }
        }
    }
}

inline void GameplayScene_handle_control_mode_switch(GameplayScene& g) {
    //control_mode mode = get_control_mode();
    ControlMode mode = g.controlmode;
    if(IsKeyPressed(KEY_C)) {
        if(mode == CONTROL_MODE_PLAYER) {
            //set_control_mode(CONTROL_MODE_CAMERA);
            g.controlmode = CONTROL_MODE_CAMERA;
        } else if(mode == CONTROL_MODE_CAMERA) {
            //      set_control_mode(CONTROL_MODE_TILE_SELECTION);

            g.controlmode = CONTROL_MODE_TILE_SELECTION;
        } else if(mode == CONTROL_MODE_TILE_SELECTION) {
            //set_control_mode(CONTROL_MODE_PLAYER);
            g.controlmode = CONTROL_MODE_PLAYER;
        }
    }
}

void GameplayScene_handle_input(GameplayScene& g) {
    if(IsKeyPressed(KEY_D)) {
        g.debug_panel_on = !g.debug_panel_on;
    }

    GameplayScene_handle_control_mode_switch(g);
    ControlMode mode = g.controlmode;
    switch(mode) {
    case CONTROL_MODE_CAMERA:
        GameplayScene_handle_camera_input(g);
        break;
    case CONTROL_MODE_PLAYER:
        GameplayScene_handle_player_input(g);
        break;
    case CONTROL_MODE_TILE_SELECTION:
        GameplayScene_handle_tile_selection_input(g);
        break;
    default:
        break;
    }
}

void GameplayScene_set_scale(GameplayScene& g, const float f) {
    if(f > 0) {

        //set_global_scale(f);
        g.global_scale = f;

        for(size_t i = 0; i < g.sprites.size(); i++) {
            //g.sprites[i]->set_scale(f);
            Sprite_set_scale(g.sprites[i], f);
        }

        //for(Sprite& s : g.sprites) {
        //s.second->set_scale(f);
        //    Sprite_set_scale(s.second, f);
        //}
    }
}

void GameplayScene_init_dungeon_floor(GameplayScene& g) {
    // these are hardcoded values, but we would prefer to do this
    // algorithmically and intelligently the columns, for now, represent walls
    // that sit on top of tiles
    GameplayScene_handle_spawn_column(g, (Vector2){2, 2});
    GameplayScene_handle_spawn_column(g, (Vector2){2, 3});
    GameplayScene_handle_spawn_column(g, (Vector2){2, 4});
    GameplayScene_handle_spawn_column(g, (Vector2){2, 5});

    GameplayScene_handle_spawn_column(g, (Vector2){3, 2});

    GameplayScene_handle_spawn_column(g, (Vector2){4, 2});
    GameplayScene_handle_spawn_column(g, (Vector2){4, 3});
    GameplayScene_handle_spawn_column(g, (Vector2){4, 5});

    // attempt to handle_spawn a second column at the same location and look for
    // an error message
    GameplayScene_handle_spawn_column(g, (Vector2){4, 5});
}

const Vector2 GameplayScene_get_start_location(GameplayScene& g) {
    return (Vector2){1, 1};
}

bool GameplayScene_init(GameplayScene& g) {
    if(!g.has_been_initialized) {
        mPrint("Initializing gameplay scene...");
        mPrint("Initializing camera...");
        GameplayScene_set_camera_default_values(g);
        mPrint("Loading assets...");
        GameplayScene_load_fonts(g);
        bool result = GameplayScene_load_textures(g);
        if(!result) {
            mPrint("Error loading textures. Exiting...");
            return false;
        }
        GameplayScene_init_dungeon_floor(g);
        mPrint("Spawning player...");
        Vector2 start_loc = GameplayScene_get_start_location(g);
        EntityId spawn_player_result = GameplayScene_spawn_player(g, start_loc);
        if(spawn_player_result < 0) {
            mPrint("Error spawning player...");
            mPrint("Location: " + to_string(start_loc.x) + ", " + to_string(start_loc.y));
            mPrint("Exiting...");
            return false;
        }
        // set the player's dungeon position
        mPrint("Setting camera target...");
        // we want to lock the camera to the player in the center of the screen
        // get_camera2d().target = dungeon_floor.get_entity_position(player_id);
        // these values are hardcoded, but we would prefer to do this
        // algorithmically
        g.camera2d.target.x = -770;
        g.camera2d.target.y = -400;
        g.prev_tile_click_zoom_level = g.global_scale;
        g.tile_click_zoom_level = g.global_scale;

        //g.message_pane = make_shared<MessagePane>(g.global_font,
        //                                          g.global_font.baseSize,
        //                                          14,
        //                                          GetScreenWidth() - 500 - 10,
        //                                          500 + 30,
        //                                          500,
        //                                          500,
        //                                          g.dungeon_events,
        //                                          g.dungeon_floor);

        mPrint("Loading sound effects...");
        g.has_been_initialized = true;
        mPrint("GameplayScene initialized");
    }
    return true;
}

const EntityId GameplayScene_spawn_player(GameplayScene& g, const Vector2 pos) {
    // make sure we spawn only 1 player
    if(g.player_id >= 0) {
        return g.player_id;
    }

    if(Vector2Equals(pos, (Vector2){-1, -1}))
        return -1;

    EntityId id =
        GameplayScene_spawn_entity(g, "player", 0, 0, SPRITETYPE_PLAYER, true, g.global_scale);
    g.player_id = id;

    //shared_ptr<Entity> player = make_shared<Entity>(id, ENTITY_PLAYER, "player");
    Entity player;
    Entity_create(player, id, ENTITY_PLAYER, "player");

    //g.dungeon_floor.add_entity_at(player, pos);
    DungeonFloor_add_entity_at(g.dungeon_floor, player, pos);
    return id;
}

const EntityId GameplayScene_spawn_column(GameplayScene& g, const Vector2 pos) {
    if(Vector2Equals(pos, (Vector2){-1, -1}))
        return -1;
    //if(g.dungeon_floor.loc_contains_entity_type(pos, ENTITY_WALL))
    if(DungeonFloor_loc_contains_entity_type(g.dungeon_floor, pos, ENTITY_WALL))
        return -1;

    EntityId id =
        GameplayScene_spawn_entity(g, "column", 0, 0, SPRITETYPE_WALL, true, g.global_scale);

    //shared_ptr<Entity> column = make_shared<Entity>(id, ENTITY_WALL, "wall");
    Entity column;
    Entity_create(column, id, ENTITY_WALL, "wall");
    //g.dungeon_floor.add_entity_at(column, pos);
    DungeonFloor_add_entity_at(g.dungeon_floor, column, pos);

    return id;
}

void GameplayScene_handle_spawn_column(GameplayScene& g, const Vector2 p) {
    if(GameplayScene_spawn_column(g, p) < 0) {
        mPrint("Error spawning column at " + to_string(p.x) + ", " + to_string(p.y));
    }
}

const EntityId GameplayScene_spawn_torch(GameplayScene& g, const Vector2 pos) {

    if(Vector2Equals(pos, (Vector2){-1, -1}))
        return -1;
    // get the tile at the last clicked tile and get its entities
    //vector<EntityId> entities = g.dungeon_floor.get_entities(pos.x, pos.y);
    const vector<EntityId>& entities = DungeonFloor_get_entities(g.dungeon_floor, pos.x, pos.y);
    bool can_place = true;
    for(auto entity_id : entities) {
        //if(g.sprites[entity_id]->get_type() == SPRITETYPE_WALL) {
        if(Sprite_get_type(g.sprites[entity_id]) == SPRITETYPE_WALL) {
            can_place = false;
        }
    }
    if(!can_place) {
        return -1;
    }

    EntityId id =
        GameplayScene_spawn_entity(g, "torch", 0, 0, SPRITETYPE_ITEM, true, g.global_scale);
    // dungeon_floor.set_entity_on_tile_with_type(id, ENTITY_TORCH, pos);
    //shared_ptr<Entity> torch = make_shared<Entity>(id, ENTITY_TORCH, "torch");
    Entity torch;
    Entity_create(torch, id, ENTITY_TORCH, "torch");
    DungeonFloor_add_entity_at(g.dungeon_floor, torch, pos);
    GameplayScene_increase_lighting_at(g, pos, 3, 3);
    return id;
}

inline void GameplayScene_draw_debug_panel(GameplayScene& g) {
    string camera_info_str =
        "Current Frame: " + to_string(g.current_frame) + "\n" +
        "Control mode: " + to_string(g.controlmode) + "\n" +
        "Camera target: " + to_string(g.camera2d.target.x) + ", " + to_string(g.camera2d.target.y) +
        "\n" + "Sprites: " + to_string(g.sprites.size()) + "\n" +
        "Textures: " + to_string(g.textures.size()) + "\n" + "IsPaused: " + to_string(g.is_paused) +
        "\n" + "Global Scale: " + to_string(g.global_scale) + "\n" +
        "Tile Click Zoom Level: " + to_string(g.tile_click_zoom_level) + "\n" +
        "Prev Tile Click Zoom Level: " + to_string(g.prev_tile_click_zoom_level) + "\n";
    const float pad = 10;
    const int w = 500;
    const int h = 300;
    const float x = pad * 2;
    const float y = GetScreenHeight() - h - 50 + pad;
    const Color c0 = Fade(BLACK, 0.5f);
    const Color c1 = WHITE;
    const Color c2 = GRAY;
    const int fontsize = g.global_font.baseSize;
    const float alpha = 0.5f;
    const Vector2 loc = (Vector2){x + pad, y + pad};
    DrawRectangle(x, y, w, h, c0);
    DrawTextEx(g.global_font, camera_info_str.c_str(), loc, fontsize, alpha, c1);
    DrawRectangleLines(x, y, w, h, c2);
}

inline void GameplayScene_draw_controls(GameplayScene& g) {
    const int fontsize = g.global_font.baseSize;
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
    DrawTextEx(
        g.global_font, s.c_str(), (Vector2){(float)x + pad, (float)y + pad}, fontsize, 0.5f, c1);
}

inline void GameplayScene_draw_hud(GameplayScene& g) {
    // draw a black box on the right side of the screen
    const int pad = 10;
    const int w = 500;
    const int h = GetScreenHeight() / 4 + pad * 3;
    const float x = GetScreenWidth() - w - pad;
    const float y = pad;
    const int fontsize = g.global_font.baseSize;
    const Color c0 = Fade(BLACK, 0.5f);
    Vector2 text_pos = (Vector2){x + pad, y + pad};
    string tile_type_str = "TILE_NONE";
    string tile_light_level_str = "";
    string s = "";

    DrawRectangle(x, y, w, h, c0);
    // get the tile type string of the last tile clicked
    if(g.last_tile_click_pos.x >= 0 && g.last_tile_click_pos.y >= 0) {
        tile_type_str =
            //g.dungeon_floor.get_tile_by_col_row(g.last_tile_click_pos.x, g.last_tile_click_pos.y)
            //    .get_type_str();

            //Tile_get_type_str(g.dungeon_floor.get_tile_by_col_row(g.last_tile_click_pos.x, g.last_tile_click_pos.y));
            Tile_get_type_str(DungeonFloor_get_tile_by_col_row(
                g.dungeon_floor, g.last_tile_click_pos.x, g.last_tile_click_pos.y));

        tile_light_level_str = to_string(
            //g.dungeon_floor.get_tile_by_col_row(g.last_tile_click_pos.x, g.last_tile_click_pos.y)
            //    .get_light_level());
            //Tile_get_light_level(g.dungeon_floor.get_tile_by_col_row(g.last_tile_click_pos.x,
            Tile_get_light_level(DungeonFloor_get_tile_by_col_row(
                g.dungeon_floor, g.last_tile_click_pos.x, g.last_tile_click_pos.y)));
    }

    // draw some text
    s += "Name: darkmage\n";
    s += "Level: 1\n";
    s += "Race: human\n";
    s += "Class: wizard\n";
    s += "HP: 1/1\n";
    s += "Stats: [10,10,10,10,10,10]\n";
    //s += "Position: " + to_string((int)g.dungeon_floor.get_entity_position(g.player_id).x) + ", " +
    s += "Position: " +
         to_string((int)DungeonFloor_get_entity_position(g.dungeon_floor, g.player_id).x) + ", " +
         //to_string((int)g.dungeon_floor.get_entity_position(g.player_id).y) + "\n";
         to_string((int)DungeonFloor_get_entity_position(g.dungeon_floor, g.player_id).y) + "\n";
    s += "Camera: " + to_string((int)g.camera2d.target.x) + ", " +
         to_string((int)g.camera2d.target.y) + "\n";
    s += "Turn: " + to_string(g.turn_count) + "\n";
    s += "Last Tile Click: " + to_string((int)g.last_tile_click_pos.x) + ", " +
         to_string((int)g.last_tile_click_pos.y) + "\n";
    s += "Tile Type Clicked: " + tile_type_str + "\n";
    s += "Tile Light Level: " + tile_light_level_str + "\n";
    s += "Control Mode: " + to_string(g.controlmode) + "\n";
    s += "\n";

    DrawRectangleLines(x, y, w, h, GRAY);
    DrawTextEx(g.global_font, s.c_str(), text_pos, fontsize, 0.5f, WHITE);
}

void GameplayScene_cleanup(GameplayScene& g) {
    for(int i = 0; i < (int)g.entity_ids.size(); i++) {
        EntityId id = g.entity_ids[i];
        //if(g.sprites[id]->get_is_marked_for_deletion()) {
        if(Sprite_get_is_marked_for_deletion(g.sprites[id])) {
            g.sprites.erase(id);
            // also need to erase from entity_ids
            g.entity_ids.erase(g.entity_ids.begin() + i);
            // update the index so that we don't skip any
            i--;
        }
    }
}

void GameplayScene_draw(GameplayScene& g) {
    BeginMode2D(g.camera2d);
    Color clear_color = BLACK;
    ClearBackground(clear_color);

    const int gridsize = DungeonFloor_get_gridsize(g.dungeon_floor);

    //   draw all tiles first
    //   entities existing on tiles will get drawn
    for(int i = 0; i < gridsize; i++) {
        for(int j = 0; j < gridsize; j++) {
            //const TileType t = g.dungeon_floor.get_tile_type(i, j);
            const TileType t = DungeonFloor_get_tile_type(g.dungeon_floor, i, j);
            const string s = GameplayScene_tile_key_for_type(g, t);
            GameplayScene_draw_tile(g, s, i, j);
        }
    }

    EndMode2D();

    if(g.hud_on) {
        GameplayScene_draw_hud(g);
    }

    if(g.debug_panel_on) {
        // DrawFPS(10, GetScreenHeight() - 50);
        GameplayScene_draw_debug_panel(g);
    }

    //if(g.display_message_log) {
    //    g.message_pane->draw();
    //}

    GameplayScene_draw_controls(g);
    GameplayScene_handle_popup_manager(g);
    g.current_frame++;
}

const string GameplayScene_tile_key_for_type(GameplayScene& g, const TileType t) {
    string tile_key = "tile-";
    switch(t) {
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

inline void
GameplayScene_draw_tile(GameplayScene& g, const string tile_key, const int i, const int j) {
    shared_ptr<TextureInfo> t = g.textures[tile_key];
    const int t_sz = t->texture.width;
    const float scale = g.global_scale;
    Rectangle src = {0, 0, (float)t->texture.width, (float)t->texture.height};
    Rectangle dest = {i * t_sz * scale,
                      j * t_sz * scale - (t->texture.height * scale - t_sz * scale),
                      t->texture.width * scale,
                      t->texture.height * scale};
    Vector2 origin = {0, 0};
    Color color = WHITE;

    //Tile& tile = g.dungeon_floor.get_tile_by_col_row(i, j);
    Tile& tile = DungeonFloor_get_tile_by_col_row(g.dungeon_floor, i, j);

    //const float light_incr = 1.0f / tile.get_max_light_level();
    const float light_incr = 1.0f / Tile_get_max_light_level(tile);
    //float alpha = 1.0f - tile.get_light_level() * light_incr;
    float alpha = 1.0f - Tile_get_light_level(tile) * light_incr;
    DrawTexturePro(t->texture, src, dest, origin, 0.0f, color);
    // now, for an upgrade, we want to draw the entities on the tile before we
    // draw the shade box this way, the entities will be drawn below the shade box
    // draw the entities on the tile
    //const vector<EntityId>& entities = g.dungeon_floor.get_entities(i, j);
    const vector<EntityId>& entities = DungeonFloor_get_entities(g.dungeon_floor, i, j);
    for(auto& e : entities) {
        //g.sprites[e]->draw();
        Sprite_draw(g.sprites[e]);
    }

    // shading for lighting
    DrawRectangle(dest.x, dest.y, dest.width, dest.height, Fade(BLACK, alpha));

    //  check to see if we need to 'select' the tile
    GameplayScene_handle_tile_click(g, dest, i, j);
}

inline void
GameplayScene_handle_tile_click(GameplayScene& g, const Rectangle dest, const int i, const int j) {
    if(g.tile_is_selected) {
        if(i == g.last_tile_click_pos.x && j == g.last_tile_click_pos.y) {
            DrawRectangleLinesEx(dest, 4, GREEN);
        }
        // else if (CheckCollisionPointRec(last_mouse_click_pos, dest)) {
        //   last_tile_click_pos = (Vector2){(float)i, (float)j};
        //  increase the light level of the tile
        //}
    }
}

inline void GameplayScene_handle_popup_manager(GameplayScene& g) {
    if(g.show_test_popup) {
        //if(g.popup_manager != nullptr) {
        //const Vector2 dest_vector = (Vector2){g.sprites[g.player_id]->get_dest().x - 20,
        //                                      g.sprites[g.player_id]->get_dest().y - 40};

        const Vector2 dest_vector = (Vector2){Sprite_get_dest(g.sprites[g.player_id]).x - 20,
                                              Sprite_get_dest(g.sprites[g.player_id]).y - 40};

        Vector2 s = GetWorldToScreen2D(dest_vector, g.camera2d);
        // Get the screen space position for
        // a 2d camera world space position
        //g.popup_manager->draw(s.x, s.y);
        PopupManager_draw(g.popup_manager, s.x, s.y);
        //}
    }
}

void GameplayScene_close(GameplayScene& g) {
    mPrint("Closing gameplay scene...");
    mPrint("Unloading textures...");
    for(auto& t : g.textures) {
        UnloadTexture(t.second->texture);
    }
    mPrint("Clearing textures...");
    g.textures.clear();
    mPrint("Clearing sprites...");
    g.sprites.clear();
    mPrint("Clearing entity ids...");
    g.entity_ids.clear();
    mPrint("Unloading font...");
    UnloadFont(g.global_font);
    // if (music != NULL) {
    //  stop music
    //  Mix_PauseMusic();
    //  Mix_FreeMusic(music);
    //}
    g.has_been_initialized = false;
    g.player_id = -1;
    g.popup_manager.alpha = 0;
    mPrint("Scene closed.");
}

void GameplayScene_set_camera_default_values(GameplayScene& g) {

    g.camera2d.offset = (Vector2){0, 0};
    g.camera2d.rotation = 0.0f;
    g.camera2d.zoom = 1.0f;
    g.camera2d.target = (Vector2){0, 0};
}

void GameplayScene_load_fonts(GameplayScene& g) {
    g.global_font = LoadFontEx("fonts/hack.ttf", g.global_font_size, 0, 250);
}

bool GameplayScene_load_textures(GameplayScene& g) {
    FILE* file = nullptr;
    char line[256];
    char *asset_name = nullptr, *num_frames = nullptr, *contexts = nullptr, *is_player = nullptr,
         *width = nullptr, *height = nullptr, *asset_path = nullptr;
    bool retval = false;
    if(g.texture_filepath == "") {
        mPrint("Error: texture_filepath not set.");
    } else {
        file = fopen(g.texture_filepath.c_str(), "r");
        if(file == NULL) {
            mPrint("Error opening file: " + g.texture_filepath);
        } else {
            while(fgets(line, sizeof(line), file)) {
                // if the line begins with a hashtag, it is a comment
                if(line[0] == '#') {
                    continue;
                }
                asset_name = strtok(line, " ");
                num_frames = strtok(NULL, " ");
                contexts = strtok(NULL, " ");
                is_player = strtok(NULL, " ");
                width = strtok(NULL, " ");
                height = strtok(NULL, " ");
                asset_path = strtok(NULL, " ");
                // check to see if asset path has newline at end
                if(asset_path[strlen(asset_path) - 1] == '\n') {
                    asset_path[strlen(asset_path) - 1] = '\0';
                }
                bool result = GameplayScene_load_texture(g,
                                                         asset_name,
                                                         asset_path,
                                                         atoi(num_frames),
                                                         atoi(contexts),
                                                         atoi(is_player),
                                                         atoi(width),
                                                         atoi(height));
                if(!result) {
                    mPrint("Error loading texture: " + string(asset_path));
                }
            }
            fclose(file);
            retval = true;
        }
    }
    return retval;
}

bool GameplayScene_load_texture(GameplayScene& g,
                                const char* asset_name,
                                const char* asset_path,
                                const int num_frames,
                                const int contexts,
                                const int is_player,
                                const int width,
                                const int height) {
    string asset_name_str = string(asset_name);
    Texture2D t = LoadTexture(asset_path);
    TextureInfo tx;
    if(t.id == 0) {
        mPrint("Error loading texture: " + string(asset_path));
        return false;
    }
    if(asset_name_str.find("tile-") != string::npos) {
        //  apply custom dithering
        Image img = LoadImageFromTexture(t);
        ImageDither(&img, 4, 4, 4, 4);
        Texture2D tt = LoadTextureFromImage(img);
        UnloadImage(img);
        t = tt;
    }
    tx.texture = t;
    tx.num_frames = num_frames;
    tx.contexts = contexts;
    tx.is_player = is_player;
    tx.asset_path = asset_path;
    tx.width = width;
    tx.height = height;
    g.textures[asset_name] = make_shared<TextureInfo>(tx);
    return true;
}

EntityId GameplayScene_spawn_entity(GameplayScene& g,
                                    const char* key,
                                    const float x,
                                    const float y,
                                    const SpriteType type,
                                    const bool is_anim,
                                    const float scale) {

    if(g.textures.find(key) == g.textures.end()) {
        mPrint("Error: texture not found: " + string(key));
        return -1;
    }

    //shared_ptr<Sprite> s = make_shared<Sprite>(g.textures[key]->texture,
    //                                           g.textures[key]->num_frames,
    //                                           g.textures[key]->contexts,
    //                                           x,
    //                                           y,
    //                                           g.textures[key]->width,
    //                                           g.textures[key]->height,
    //                                           type);

    Sprite s;
    Sprite_create(s,
                  g.textures[key]->texture,
                  g.textures[key]->num_frames,
                  g.textures[key]->contexts,
                  x,
                  y,
                  g.textures[key]->width,
                  g.textures[key]->height,
                  type);

    //if(s == nullptr) {
    //    mPrint("Error creating sprite.");
    //    return -1;
    //}
    //s->set_scale(scale);
    Sprite_set_scale(s, scale);
    //s->set_is_animating(is_anim);
    Sprite_set_is_animating(s, is_anim);
    g.sprites[next_entity_id] = s;
    g.entity_ids.push_back(next_entity_id);
    return next_entity_id++;
}

EntityId GameplayScene_spawn_entity(GameplayScene& g,
                                    const char* key,
                                    const float x,
                                    const float y,
                                    const SpriteType type,
                                    const bool is_anim) {
    return GameplayScene_spawn_entity(g, key, x, y, type, is_anim, g.global_scale);
}

void GameplayScene_set_id(GameplayScene& g, SceneId id) {
    g.id = id;
}

//void GameplayScene_set_popup_manager(GameplayScene& t, shared_ptr<PopupManager> pm) {
//    t.popup_manager = pm;
//}
