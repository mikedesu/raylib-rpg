#pragma once

typedef enum {
  TILE_VOID,
  TILE_FLOOR_BASIC,
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

private:
  tile_type type = TILE_VOID;
};
