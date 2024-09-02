#pragma once

#include "Entity.h"
#include "EntityId.h"
#include "Tile.h"
#include "raylib.h"
#include <algorithm>
#include <memory>
#include <unordered_map>

using namespace std;

typedef struct {
    const int gridsize = 16;
    Tile grid[16][16];
    unordered_map<EntityId, Vector2> entity_positions;
    unordered_map<EntityId, Entity> entities;
    //unordered_map<EntityId, shared_ptr<Entity>> entities;
} DungeonFloor;

void DungeonFloor_create(DungeonFloor& d);
void DungeonFloor_destroy(DungeonFloor& d);
void DungeonFloor_set_tile_type(DungeonFloor& d,
                                const int col,
                                const int row,
                                const TileType value);
void DungeonFloor_set_tile_type_all(DungeonFloor& d, const TileType value);
const int DungeonFloor_get_gridsize(DungeonFloor& d);
const TileType DungeonFloor_get_tile_type(DungeonFloor& d, const int col, const int row);
const Vector2 DungeonFloor_get_entity_position(DungeonFloor& d, const EntityId id);
Tile& DungeonFloor_get_tile_by_col_row(DungeonFloor& d, const int col, const int row);
Tile& DungeonFloor_get_tile_by_vec(DungeonFloor& d, const Vector2 position);
const vector<EntityId>& DungeonFloor_get_entities(DungeonFloor& d, const int col, const int row);
void DungeonFloor_remove_entity(DungeonFloor& d, const EntityId id);
void DungeonFloor_remove_entity(DungeonFloor& d, Tile& t, const EntityId id);

void DungeonFloor_remove_entity_from_tile(DungeonFloor& d,
                                          const EntityId id,
                                          const Vector2 position);

const EntityType DungeonFloor_get_entity_type(DungeonFloor& d, const EntityId id);

const bool
DungeonFloor_loc_contains_entity_type(DungeonFloor& d, const Vector2 loc, const EntityType type);
const string DungeonFloor_get_entity_name(DungeonFloor& d, const EntityId id);
void DungeonFloor_add_entity_at(DungeonFloor& d, Entity& entity, const Vector2 position);
const bool DungeonFloor_move_entity_to_tile(DungeonFloor& d, EntityId id, const Vector2 t_pos);

//    void set_tile_type(const int col, const int row, const TileType value);
//    void set_tile_type_all(const TileType value);
//    const int get_gridsize() const;
//    const TileType get_tile_type(const int col, const int row) const;
//    const Vector2 get_entity_position(const EntityId id);
//    Tile& get_tile_by_col_row(const int col, const int row);
//    Tile& get_tile_by_vec(const Vector2 position);
//    const vector<EntityId>& get_entities(const int col, const int row) const;
//    void remove_entity(const EntityId id);
//    void remove_entity_from_tile(const EntityId id, const Vector2 position);
//    const EntityType get_entity_type(const EntityId id) const;
//    const bool loc_contains_entity_type(const Vector2 loc, const EntityType type) const;
//    const string get_entity_name(const EntityId id) const;
//    void add_entity_at(shared_ptr<Entity> entity, const Vector2 position);
//    const bool move_entity_to_tile(EntityId id, const Vector2 position);
