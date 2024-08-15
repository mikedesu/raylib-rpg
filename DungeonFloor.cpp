#include "DungeonFloor.h"

DungeonFloor::DungeonFloor() {
  set_tile_type_all(TILE_FLOOR_BASIC);

  for (int i = 0; i < gridsize; i++) {
    set_tile_type(i, 0, TILE_FLOOR_WOOD);
    set_tile_type(i, 1, TILE_FLOOR_DIRT);
  }

  // for (int i = 0; i < gridsize; i++) {
  // }
}

DungeonFloor::~DungeonFloor() {}

// void DungeonFloor::set_tile_type_all(const tile_type value) {
//   for (int i = 0; i < gridsize; i++) {
//     for (int j = 0; j < gridsize; j++) {
//       set_tile_type(i, j, value);
//     }
//   }
// }

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

// void DungeonFloor::set_tile_type(const int col, const int row,
//                                  const tile_type value) {
//
// }

void DungeonFloor::set_entity_position(const entity_id id,
                                       const Vector2 position) {

  // if the entity is not in the map, add it
  if (entity_positions.find(id) == entity_positions.end()) {
    entity_positions[id] = position;
    // add the entity to the tile
    grid[(int)position.x][(int)position.y].add_entity(id);
  } else {
    // save the old position
    Vector2 old_position = entity_positions[id];
    // if the entity is in the map, update its position
    entity_positions[id] = position;
    // move the entity from the old tile to the new tile
    Tile &old_tile = grid[(int)old_position.x][(int)old_position.y];
    Tile &new_tile = grid[(int)position.x][(int)position.y];
    old_tile.move_entity_to_tile(id, new_tile);
    // grid[(int)old_position.x][(int)old_position.y].move_entity_to_tile(id,
    //                                                                   grid[(int)position.x][(int)position.y]);
  }
}

const Vector2 DungeonFloor::get_entity_position(const entity_id id) {
  return entity_positions[id];
}
