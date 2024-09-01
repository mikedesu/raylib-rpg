#include "DungeonFloor.h"

DungeonFloor::DungeonFloor() {
    set_tile_type_all(TILE_FLOOR_BASIC);
    for(int i = 0; i < gridsize; i++) {
        set_tile_type(i, 0, TILE_FLOOR_WOOD);
        set_tile_type(i, 1, TILE_FLOOR_DIRT);
    }
    set_tile_type(0, 0, TILE_FLOOR_UPSTAIRS);
    set_tile_type(7, 7, TILE_FLOOR_DOWNSTAIRS);
}

DungeonFloor::~DungeonFloor() { }

void DungeonFloor::set_tile_type(const int col, const int row, const TileType value) {
    if(row < 0 || row >= gridsize || col < 0 || col >= gridsize) {
        return;
    }
    Tile_set_type(grid[col][row], value);
}

void DungeonFloor::set_tile_type_all(const TileType value) {
    for(int i = 0; i < gridsize; i++) {
        for(int j = 0; j < gridsize; j++) {
            set_tile_type(i, j, value);
        }
    }
}

const int DungeonFloor::get_gridsize() const {
    return gridsize;
}

const TileType DungeonFloor::get_tile_type(const int col, const int row) const {
    if(row < 0 || row >= gridsize || col < 0 || col >= gridsize) {
        return TILE_NONE;
    }
    return grid[col][row].type;
}

const Vector2 DungeonFloor::get_entity_position(const EntityId id) {
    if(entity_positions.find(id) == entity_positions.end()) {
        return Vector2{-1, -1};
    }
    return entity_positions[id];
}

Tile& DungeonFloor::get_tile_by_col_row(const int col, const int row) {
    return grid[col][row];
}

Tile& DungeonFloor::get_tile_by_vec(const Vector2 position) {
    return grid[(int)position.x][(int)position.y];
}

const vector<EntityId>& DungeonFloor::get_entities(const int col, const int row) const {
    return grid[col][row].entities;
}

void DungeonFloor::remove_entity(const EntityId id) {
    remove_entity_from_tile(id, get_entity_position(id));
}

void DungeonFloor::remove_entity_from_tile(const EntityId id, const Vector2 position) {
    Tile_remove_entity(grid[(int)position.x][(int)position.y], id);
    entity_positions.erase(id);
    entities.erase(id);
}

const EntityType DungeonFloor::get_entity_type(const EntityId id) const {

    if(entities.find(id) == entities.end()) {
        return ENTITY_NONE;
    }
    return entities.at(id)->get_type();
}

const bool DungeonFloor::loc_contains_entity_type(const Vector2 loc, const EntityType type) const {
    const vector<EntityId>& entities = get_entities(loc.x, loc.y);
    auto pred = [this, type](EntityId id) { return get_entity_type(id) == type; };
    return find_if(entities.begin(), entities.end(), pred) != entities.end();
}

const string DungeonFloor::get_entity_name(const EntityId id) const {
    return entities.at(id)->get_name();
}

void DungeonFloor::add_entity_at(shared_ptr<Entity> entity, const Vector2 position) {
    EntityId id = entity->get_id();
    int x = (int)position.x;
    int y = (int)position.y;
    if(entity_positions.find(id) == entity_positions.end()) {
        entity_positions[id] = position;
        entities[id] = entity;
        //grid[x][y].add_entity(id);
        Tile_add_entity(grid[x][y], id);
    } else {
        move_entity_to_tile(id, position);
    }
}

const bool DungeonFloor::move_entity_to_tile(EntityId id, const Vector2 t_pos) {
    // can only move under certain conditions:
    // the entity is in this tile
    // the entity is not already in the destination tile
    // the destination tile does not contain enemies
    // etc
    // get the source tile
    Vector2 position = get_entity_position(id);
    Tile& tile_src = grid[(int)position.x][(int)position.y];
    Tile& tile_dst = grid[(int)t_pos.x][(int)t_pos.y];
    Tile_remove_entity(tile_src, id);
    Tile_add_entity(tile_dst, id);
    // update the entity's position
    entity_positions[id] = t_pos;
    return true;
}
