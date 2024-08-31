#pragma once

#include "Entity.h"
#include "Tile.h"
#include "entity_id.h"
#include "raylib.h"
#include <algorithm>
#include <memory>
#include <unordered_map>

using namespace std;

class DungeonFloor {
private:
    const int gridsize = 16;
    Tile grid[16][16];
    unordered_map<entity_id, Vector2> entity_positions;
    unordered_map<entity_id, shared_ptr<Entity>> entities;

public:
    DungeonFloor() {
        set_tile_type_all(TILE_FLOOR_BASIC);
        for(int i = 0; i < gridsize; i++) {
            set_tile_type(i, 0, TILE_FLOOR_WOOD);
            set_tile_type(i, 1, TILE_FLOOR_DIRT);
        }
        set_tile_type(0, 0, TILE_FLOOR_UPSTAIRS);
        set_tile_type(7, 7, TILE_FLOOR_DOWNSTAIRS);
    }
    ~DungeonFloor() { }

    void set_tile_type(const int col, const int row, const TileType value) {
        if(row < 0 || row >= gridsize || col < 0 || col >= gridsize) {
            return;
        }
        grid[col][row].set_type(value);
    }

    void set_tile_type_all(const TileType value) {
        for(int i = 0; i < gridsize; i++) {
            for(int j = 0; j < gridsize; j++) {
                set_tile_type(i, j, value);
            }
        }
    }

    const int get_gridsize() const {
        return gridsize;
    }
    const TileType get_tile_type(const int col, const int row) const {
        if(row < 0 || row >= gridsize || col < 0 || col >= gridsize) {
            return TILE_NONE;
        }
        return grid[col][row].get_type();
    }
    const Vector2 get_entity_position(const entity_id id) {
        if(entity_positions.find(id) == entity_positions.end()) {
            return Vector2{-1, -1};
        }
        return entity_positions[id];
    }

    Tile& get_tile_by_col_row(const int col, const int row) {
        return grid[col][row];
    }
    Tile& get_tile_by_vec(const Vector2 position) {
        return grid[(int)position.x][(int)position.y];
    }

    const vector<entity_id>& get_entities(const int col, const int row) const {
        return grid[col][row].get_entities();
    }

    void remove_entity(const entity_id id) {
        remove_entity_from_tile(id, get_entity_position(id));
    }

    void remove_entity_from_tile(const entity_id id, const Vector2 position) {
        grid[(int)position.x][(int)position.y].remove_entity(id);
        entity_positions.erase(id);
        entities.erase(id);
    }

    const EntityType get_entity_type(const entity_id id) const {

        if(entities.find(id) == entities.end()) {
            return ENTITY_NONE;
        }
        return entities.at(id)->get_type();
    }

    const bool loc_contains_entity_type(const Vector2 loc, const EntityType type) const {
        const vector<entity_id>& entities = get_entities(loc.x, loc.y);
        auto pred = [this, type](entity_id id) { return get_entity_type(id) == type; };
        return find_if(entities.begin(), entities.end(), pred) != entities.end();
    }

    const string get_entity_name(const entity_id id) const {
        return entities.at(id)->get_name();
    }

    void add_entity_at(shared_ptr<Entity> entity, const Vector2 position) {
        entity_id id = entity->get_id();
        int x = (int)position.x;
        int y = (int)position.y;
        if(entity_positions.find(id) == entity_positions.end()) {
            entity_positions[id] = position;
            entities[id] = entity;
            grid[x][y].add_entity(id);
        } else {
            move_entity_to_tile(id, position);
        }
    }

    const bool move_entity_to_tile(entity_id id, const Vector2 position);
};
