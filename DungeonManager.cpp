#include "DungeonManager.h"

DungeonManager::DungeonManager() {
  for (int j = 0; j < gridsize; j++) {
    for (int i = j % 2; i < gridsize; i += 2) {
      grid[i][j] = 1;
    }
  }
}

DungeonManager::~DungeonManager() {}

const int DungeonManager::get_player_row() const { return player_row; }
const int DungeonManager::get_player_col() const { return player_col; }

void DungeonManager::set_player_row(const int row) {
  if (row < 0 || row >= gridsize) {
    return;
  }
  set_player_position(row, get_player_col());
  // player_row = row;
}

void DungeonManager::set_player_col(const int col) {
  if (col < 0 || col >= gridsize) {
    return;
  }
  set_player_position(get_player_row(), col);

  // player_col = col;
}

const int DungeonManager::get_cell(const int row, const int col) const {
  if (row < 0 || row >= gridsize || col < 0 || col >= gridsize) {
    return -1;
  }
  return grid[row][col];
}

void DungeonManager::incr_player_col() { set_player_col(get_player_col() + 1); }

void DungeonManager::decr_player_col() { set_player_col(get_player_col() - 1); }

void DungeonManager::set_player_position(const int row, const int col) {
  if (row < 0 || row >= gridsize || col < 0 || col >= gridsize) {
    return;
  }
  player_row = row;
  player_col = col;
}

void DungeonManager::incr_player_row() { set_player_row(get_player_row() + 1); }
void DungeonManager::decr_player_row() { set_player_row(get_player_row() - 1); }

const int DungeonManager::get_gridsize() const { return gridsize; }
