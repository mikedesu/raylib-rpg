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
  void set_entity_position(const entity_id id, const Vector2 position);

  const int get_gridsize() const;
  const tile_type get_tile_type(const int col, const int row) const;
  const Vector2 get_entity_position(const entity_id id);
  const bool move_entity_to_tile(entity_id id, const Vector2 position);

  Tile &get_tile_ref(const int col, const int row);
  Tile &get_tile_ref(const Vector2 position);

  const vector<entity_id> &get_entities(const int col, const int row) const;

private:
  const int gridsize = 8;
  Tile grid[8][8];

  unordered_map<entity_id, Vector2> entity_positions;
};
