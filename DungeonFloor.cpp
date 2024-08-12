#include "DungeonFloor.h"

DungeonFloor::DungeonFloor() { set_grid_all(TILE_FLOOR_BASIC); }

DungeonFloor::~DungeonFloor() {}

// void DungeonFloor::set_grid_all(const tile_type value) {
//   for (int i = 0; i < gridsize; i++) {
//     for (int j = 0; j < gridsize; j++) {
//       set_grid(i, j, value);
//     }
//   }
// }

void DungeonFloor::set_grid(const int col, const int row,
                            const tile_type value) {
  if (row < 0 || row >= gridsize || col < 0 || col >= gridsize) {
    return;
  }
  grid[col][row].set_type(value);
}

void DungeonFloor::set_grid_all(const tile_type value) {
  for (int i = 0; i < gridsize; i++) {
    for (int j = 0; j < gridsize; j++) {
      set_grid(i, j, value);
    }
  }
}
