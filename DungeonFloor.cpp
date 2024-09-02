#include "DungeonFloor.h"

void DungeonFloor_DungeonFloor(DungeonFloor& d) {
    DungeonFloor_set_tile_type_all(d, TILE_FLOOR_BASIC);
    for(int i = 0; i < d.gridsize; i++) {
        DungeonFloor_set_tile_type(d, i, 0, TILE_FLOOR_WOOD);
        DungeonFloor_set_tile_type(d, i, 1, TILE_FLOOR_DIRT);
    }
    DungeonFloor_set_tile_type(d, 0, 0, TILE_FLOOR_UPSTAIRS);
    DungeonFloor_set_tile_type(d, 7, 7, TILE_FLOOR_DOWNSTAIRS);
}

void DungeonFloor_destroy(DungeonFloor& d) { }

void DungeonFloor_set_tile_type(DungeonFloor& d,
                                const int col,
                                const int row,
                                const TileType value) {
    if(row < 0 || row >= d.gridsize || col < 0 || col >= d.gridsize) {
        return;
    }
    Tile_set_type(d.grid[col][row], value);
}

void DungeonFloor_set_tile_type_all(DungeonFloor& d, const TileType value) {
    for(int i = 0; i < d.gridsize; i++) {
        for(int j = 0; j < d.gridsize; j++) {
            DungeonFloor_set_tile_type(d, i, j, value);
        }
    }
}

const int DungeonFloor_get_gridsize(DungeonFloor& d) {
    return d.gridsize;
}

const TileType DungeonFloor_get_tile_type(DungeonFloor& d, const int col, const int row) {
    if(row < 0 || row >= d.gridsize || col < 0 || col >= d.gridsize) {
        return TILE_NONE;
    }
    return d.grid[col][row].type;
}

const Vector2 DungeonFloor_get_entity_position(DungeonFloor& d, const EntityId id) {
    if(d.entity_positions.find(id) == d.entity_positions.end()) {
        return Vector2{-1, -1};
    }
    return d.entity_positions[id];
}

Tile& DungeonFloor_get_tile_by_col_row(DungeonFloor& d, const int col, const int row) {
    return d.grid[col][row];
}

Tile& DungeonFloor_get_tile_by_vec(DungeonFloor& d, const Vector2 position) {
    return d.grid[(int)position.x][(int)position.y];
}

const vector<EntityId>& DungeonFloor_get_entities(DungeonFloor& d, const int col, const int row) {
    return d.grid[col][row].entities;
}

void DungeonFloor_remove_entity(DungeonFloor& d, const EntityId id) {
    DungeonFloor_remove_entity_from_tile(d, id, d.entity_positions[id]);
}

void DungeonFloor_remove_entity(DungeonFloor& d, Tile& t, const EntityId id) {
    Tile_remove_entity(t, id);
    d.entity_positions.erase(id);
    d.entities.erase(id);
}

void DungeonFloor_remove_entity_from_tile(DungeonFloor& d,
                                          const EntityId id,
                                          const Vector2 position) {
    DungeonFloor_remove_entity(d, d.grid[(int)position.x][(int)position.y], id);
    d.entity_positions.erase(id);
    d.entities.erase(id);
}

const EntityType DungeonFloor_get_entity_type(DungeonFloor& d, const EntityId id) {

    if(d.entities.find(id) == d.entities.end()) {
        return ENTITY_NONE;
    }
    //return d.entities.at(id)->get_type();
    return Entity_get_type(d.entities[id]);
}

const bool
DungeonFloor_loc_contains_entity_type(DungeonFloor& d, const Vector2 loc, const EntityType type) {
    const vector<EntityId>& entities = DungeonFloor_get_entities(d, loc.x, loc.y);
    //auto pred = [this, type](DungeonFloor& d, EntityId id) { return get_entity_type(id) == type; };
    //return find_if(DungeonFloor & d, entities.begin(), entities.end(), pred) != entities.end();

    for(EntityId id : entities) {
        if(DungeonFloor_get_entity_type(d, id) == type) {
            return true;
        }
    }
    return false;
}

const string DungeonFloor_get_entity_name(DungeonFloor& d, const EntityId id) {
    //return d.entities.at(id).get_name();
    return Entity_get_name(d.entities[id]);
}

//void DungeonFloor_add_entity_at(DungeonFloor& d,shared_ptr<Entity> entity,const Vector2 position) {
void DungeonFloor_add_entity_at(DungeonFloor& d, Entity& entity, const Vector2 position) {

    //EntityId id = entity.get_id(DungeonFloor & d, );
    EntityId id = Entity_get_id(entity);

    int x = (int)position.x;
    int y = (int)position.y;
    if(d.entity_positions.find(id) == d.entity_positions.end()) {
        d.entity_positions[id] = position;
        d.entities[id] = entity;
        //grid[x][y].add_entity(DungeonFloor &d, id);
        Tile_add_entity(d.grid[x][y], id);
    } else {
        DungeonFloor_move_entity_to_tile(d, id, position);
    }
}

const bool DungeonFloor_move_entity_to_tile(DungeonFloor& d, EntityId id, const Vector2 t_pos) {
    // can only move under certain conditions:
    // the entity is in this tile
    // the entity is not already in the destination tile
    // the destination tile does not contain enemies
    // etc
    // get the source tile
    //Vector2 position = d.get_entity_position(d, id);
    Vector2 position = DungeonFloor_get_entity_position(d, id);
    Tile& tile_src = d.grid[(int)position.x][(int)position.y];
    Tile& tile_dst = d.grid[(int)t_pos.x][(int)t_pos.y];
    Tile_remove_entity(tile_src, id);
    Tile_add_entity(tile_dst, id);
    // update the entity's position
    d.entity_positions[id] = t_pos;
    return true;
}
