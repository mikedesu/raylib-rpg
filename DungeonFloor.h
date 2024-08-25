#pragma once

#include "Tile.h"
#include "entity_id.h"
#include "entity_type.h"
#include "raylib.h"
#include <unordered_map>

using std::unordered_map;

class DungeonFloor {

public:
  DungeonFloor();
  ~DungeonFloor();

  void set_tile_type(const int col, const int row, const tile_type value);
  void set_tile_type_all(const tile_type value);
  // void set_entity_position(const entity_id id, const Vector2 position);
  void set_entity_on_tile_with_type(const entity_id id, const entity_type type,
                                    const Vector2 position);

  const int get_gridsize() const;
  const tile_type get_tile_type(const int col, const int row) const;
  const Vector2 get_entity_position(const entity_id id);
  const bool move_entity_to_tile(entity_id id, const Vector2 position);

  Tile &get_tile_by_col_row(const int col, const int row);
  Tile &get_tile_by_vec(const Vector2 position);

  const vector<entity_id> &get_entities(const int col, const int row) const;

  // remove entity methods
  void remove_entity(const entity_id id);
  void remove_entity_from_tile(const entity_id id, const Vector2 position);

  const entity_type get_entity_type(const entity_id id) const;

private:
  const int gridsize = 16;
  Tile grid[16][16];

  unordered_map<entity_id, Vector2> entity_positions;
  unordered_map<entity_id, entity_type> entity_types;
};
