#pragma once

#include "Tile.h"
#include "entity_id.h"
#include "raylib.h"
#include <unordered_map>

using std::unordered_map;

class DungeonFloor {

public:
  DungeonFloor();
  ~DungeonFloor();

  void set_tile_type(const int col, const int row, const tile_type value);
  void set_tile_type_all(const tile_type value);

  const int get_gridsize() const;
  const tile_type get_tile_type(const int col, const int row) const;
  // void set_tile_type(const int col, const int row, const tile_type value);

  void set_entity_position(const entity_id id, const Vector2 position);
  const Vector2 get_entity_position(const entity_id id);

private:
  const int gridsize = 8;
  Tile grid[8][8];

  unordered_map<entity_id, Vector2> entity_positions;
};
