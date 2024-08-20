#pragma once

#include "Sprite.h"
#include "entity_id.h"
#include <memory>
#include <vector>

using std::shared_ptr;
using std::vector;

typedef enum {
  TILE_NONE,
  TILE_VOID,
  TILE_FLOOR_BASIC,
  TILE_FLOOR_STONE,
  TILE_FLOOR_WOOD,
  TILE_FLOOR_DIRT,
  // TILE_WALL_BASIC,
  TILE_COUNT

} tile_type;

class Tile {

public:
  Tile();
  Tile(const tile_type t);
  ~Tile();

  void set_type(const tile_type t);
  void add_entity(const entity_id id);
  void remove_entity(const entity_id id);

  const tile_type get_type() const;
  const vector<entity_id> &get_entities() const;

private:
  tile_type type = TILE_VOID;
  vector<entity_id> entities;
};
