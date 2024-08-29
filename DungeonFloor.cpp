#include "DungeonFloor.h"

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
