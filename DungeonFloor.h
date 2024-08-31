#pragma once

#include "Entity.h"
#include "EntityId.h"
#include "Tile.h"
#include "raylib.h"
#include <algorithm>
#include <memory>
#include <unordered_map>

using namespace std;

class DungeonFloor {
private:
    const int gridsize = 16;
    Tile grid[16][16];
    unordered_map<EntityId, Vector2> entity_positions;
    unordered_map<EntityId, shared_ptr<Entity>> entities;

public:
    DungeonFloor();
    ~DungeonFloor();

    void set_tile_type(const int col, const int row, const TileType value);

    void set_tile_type_all(const TileType value);

    const int get_gridsize() const;
    const TileType get_tile_type(const int col, const int row) const;
    const Vector2 get_entity_position(const EntityId id);

    Tile& get_tile_by_col_row(const int col, const int row);
    Tile& get_tile_by_vec(const Vector2 position);

    const vector<EntityId>& get_entities(const int col, const int row) const;

    void remove_entity(const EntityId id);

    void remove_entity_from_tile(const EntityId id, const Vector2 position);

    const EntityType get_entity_type(const EntityId id) const;

    const bool loc_contains_entity_type(const Vector2 loc, const EntityType type) const;

    const string get_entity_name(const EntityId id) const;

    void add_entity_at(shared_ptr<Entity> entity, const Vector2 position);

    const bool move_entity_to_tile(EntityId id, const Vector2 position);
};
