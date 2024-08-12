#include "DungeonFloor.h"

DungeonFloor::DungeonFloor() {
  set_grid_all(TILE_FLOOR_BASIC);

  set_grid(0, 1, TILE_VOID);
  set_grid(1, 1, TILE_VOID);
  set_grid(2, 1, TILE_VOID);
  set_grid(3, 1, TILE_VOID);
}

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

const int DungeonFloor::get_gridsize() const { return gridsize; }
const tile_type DungeonFloor::get_tile_type(const int col,
                                            const int row) const {

  if (row < 0 || row >= gridsize || col < 0 || col >= gridsize) {
    return TILE_NONE;
  }

  return grid[col][row].get_type();
}
void DungeonFloor::set_tile_type(const int col, const int row,
                                 const tile_type value) {}
