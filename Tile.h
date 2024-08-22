#pragma once

#include "Sprite.h"
#include "entity_id.h"

#include <memory>
#include <string>
#include <vector>

using std::shared_ptr;
using std::string;
using std::vector;

#define TILE_DEFAULT_LIGHT_LEVEL 5
#define TILE_DEFAULT_MAX_LIGHT_LEVEL 10
#define TILE_DEFAULT_MIN_LIGHT_LEVEL 0
#define TILE_DEFAULT_LIGHT_INCR 0.1f

typedef enum {
  TILE_NONE,
  TILE_VOID,
  TILE_FLOOR_BASIC,
  TILE_FLOOR_STONE,
  TILE_FLOOR_WOOD,
  TILE_FLOOR_DIRT,
  TILE_FLOOR_UPSTAIRS,
  TILE_FLOOR_DOWNSTAIRS,
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
  const string get_type_str() const;

  void set_light_level(const int level);
  void increase_light_level();
  const int get_light_level() const;
  void decrease_light_level();

private:
  tile_type type = TILE_VOID;
  vector<entity_id> entities;

  int light_level = TILE_DEFAULT_LIGHT_LEVEL;
  const float light_incr = TILE_DEFAULT_LIGHT_INCR;
  const int max_light_level = TILE_DEFAULT_MAX_LIGHT_LEVEL;
};
