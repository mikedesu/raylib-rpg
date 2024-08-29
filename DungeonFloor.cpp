#include "DungeonFloor.h"

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
