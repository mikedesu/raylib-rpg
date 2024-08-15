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
  TILE_WALL_BASIC,
  TILE_COUNT

} tile_type;

class Tile {

public:
  Tile();
  Tile(tile_type t);
  ~Tile();

  void set_type(tile_type t);
  const tile_type get_type() const;

  // void add_sprite(shared_ptr<Sprite> s);
  // void remove_sprite(shared_ptr<Sprite> s);
  // const vector<shared_ptr<Sprite>> &get_sprites() const;

  void add_entity(entity_id id);
  void remove_entity(entity_id id);
  const vector<entity_id> &get_entities() const;

  void move_entity_to_tile(entity_id id, Tile &tile);

private:
  tile_type type = TILE_VOID;

  vector<entity_id> entities;
};
