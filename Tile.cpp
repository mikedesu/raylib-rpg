#include "Tile.h"

Tile::Tile() {}

Tile::Tile(tile_type t) { set_type(t); }

Tile::~Tile() {}

void Tile::set_type(tile_type t) { type = t; }

const tile_type Tile::get_type() const { return type; }
