#include "DungeonManager.h"
#include <cassert>

DungeonManager::DungeonManager() {

  // for (int j = 0; j < gridsize; j++) {
  //   for (int i = j % 2; i < gridsize; i += 2) {
  //     grid[i][j] = 1;
  //   }
  // }

  set_grid_all(TILE_FLOOR_BASIC);

  // basic wall testing
  set_grid(1, 0, TILE_WALL_BASIC);
  set_grid(1, 1, TILE_WALL_BASIC);
  set_grid(1, 2, TILE_WALL_BASIC);
  set_grid(1, 3, TILE_WALL_BASIC);
  set_grid(1, 4, TILE_WALL_BASIC);
}

DungeonManager::~DungeonManager() {}

const int DungeonManager::get_player_row() const { return player_row; }
const int DungeonManager::get_player_col() const { return player_col; }

void DungeonManager::set_grid_all(const tile_type value) {
  for (int i = 0; i < gridsize; i++) {
    for (int j = 0; j < gridsize; j++) {
      set_grid(i, j, value);
    }
  }
}

void DungeonManager::set_grid(const int col, const int row,
                              const tile_type value) {
  if (row < 0 || row >= gridsize || col < 0 || col >= gridsize) {
    return;
  }
  // grid[row][col] = value;
  grid[col][row] = value;
}

void DungeonManager::set_player_row(const int row) {
  if (row < 0 || row >= gridsize) {
    return;
  }
  // set_player_position(row, get_player_col());
  set_player_position(get_player_col(), row);
}

void DungeonManager::set_player_col(const int col) {
  if (col < 0 || col >= gridsize) {
    return;
  }
  // set_player_position(get_player_row(), col);
  set_player_position(col, get_player_row());
}

const Tile DungeonManager::get_cell(const int col, const int row) const {
  assert(row >= 0 && row < gridsize && col >= 0 && col < gridsize);
  // return grid[row][col];
  return grid[col][row];
}

void DungeonManager::set_player_position(const int col, const int row) {
  if (row < 0 || row >= gridsize || col < 0 || col >= gridsize) {
    return;
  }

  // cannot move into a wall
  // check to see if destination is a wall
  if (grid[col][row].get_type() == TILE_WALL_BASIC) {
    return;
  }

  player_row = row;
  player_col = col;
}

void DungeonManager::incr_player_col() { set_player_col(get_player_col() + 1); }
void DungeonManager::decr_player_col() { set_player_col(get_player_col() - 1); }
void DungeonManager::incr_player_row() { set_player_row(get_player_row() + 1); }
void DungeonManager::decr_player_row() { set_player_row(get_player_row() - 1); }
const int DungeonManager::get_gridsize() const { return gridsize; }
