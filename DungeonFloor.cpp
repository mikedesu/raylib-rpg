#include "DungeonFloor.h"
#include "EntityType.h"

DungeonFloor::DungeonFloor() {
  set_tile_type_all(TILE_FLOOR_BASIC);
  for (int i = 0; i < gridsize; i++) {
    set_tile_type(i, 0, TILE_FLOOR_WOOD);
    set_tile_type(i, 1, TILE_FLOOR_DIRT);
  }
  set_tile_type(0, 0, TILE_FLOOR_UPSTAIRS);
  set_tile_type(7, 7, TILE_FLOOR_DOWNSTAIRS);
}

DungeonFloor::~DungeonFloor() {}

void DungeonFloor::set_tile_type(const int col, const int row,
                                 const tile_type value) {
  if (row < 0 || row >= gridsize || col < 0 || col >= gridsize) {
    return;
  }
  grid[col][row].set_type(value);
}

void DungeonFloor::set_tile_type_all(const tile_type value) {
  for (int i = 0; i < gridsize; i++) {
    for (int j = 0; j < gridsize; j++) {
      set_tile_type(i, j, value);
    }
  }
}

const int DungeonFloor::get_gridsize() const { return gridsize; }

const tile_type DungeonFloor::get_tile_type(const int col,
                                            const int row) const {
  if (row < 0 || row >= gridsize || col < 0 || col >= gridsize) {
    return TILE_NONE;
  }
  return grid[col][row].get_type();
}

void DungeonFloor::add_entity_at(shared_ptr<Entity> entity,
                                 const Vector2 position) {
  entity_id id = entity->get_id();
  int x = (int)position.x;
  int y = (int)position.y;
  if (entity_positions.find(id) == entity_positions.end()) {
    entity_positions[id] = position;
    entities[id] = entity;
    grid[x][y].add_entity(id);
  } else {
    move_entity_to_tile(id, position);
  }
}

const Vector2 DungeonFloor::get_entity_position(const entity_id id) {
  return entity_positions[id];
}

const bool DungeonFloor::move_entity_to_tile(entity_id id,
                                             const Vector2 t_pos) {
  // can only move under certain conditions:
  // the entity is in this tile
  // the entity is not already in the destination tile
  // the destination tile does not contain enemies
  // etc
  // get the source tile
  Vector2 position = get_entity_position(id);
  Tile &tile_src = grid[(int)position.x][(int)position.y];
  Tile &tile_dst = grid[(int)t_pos.x][(int)t_pos.y];
  tile_src.remove_entity(id);
  tile_dst.add_entity(id);
  // update the entity's position
  entity_positions[id] = t_pos;
  return true;
}

Tile &DungeonFloor::get_tile_by_col_row(const int col, const int row) {
  return grid[col][row];
}

Tile &DungeonFloor::get_tile_by_vec(const Vector2 position) {
  return grid[(int)position.x][(int)position.y];
}

const vector<entity_id> &DungeonFloor::get_entities(const int col,
                                                    const int row) const {
  return grid[col][row].get_entities();
}

void DungeonFloor::remove_entity(const entity_id id) {
  remove_entity_from_tile(id, get_entity_position(id));
}

void DungeonFloor::remove_entity_from_tile(const entity_id id,
                                           const Vector2 position) {
  grid[(int)position.x][(int)position.y].remove_entity(id);
  entity_positions.erase(id);
  // entity_types.erase(id);
  entities.erase(id);
}

const EntityType DungeonFloor::get_entity_type(const entity_id id) const {
  if (entities.find(id) == entities.end()) {
    return ENTITY_NONE;
  }
  return entities.at(id)->get_type();
}
