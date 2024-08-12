#pragma once

#include "Tile.h"

class DungeonFloor {

public:
  DungeonFloor();
  ~DungeonFloor();

  void set_grid(const int col, const int row, const tile_type value);
  void set_grid_all(const tile_type value);

  const int get_gridsize() const;
  const tile_type get_tile_type(const int col, const int row) const;
  void set_tile_type(const int col, const int row, const tile_type value);

private:
  const int gridsize = 8;
  Tile grid[8][8];
};
