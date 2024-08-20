#include "Tile.h"

Tile::Tile() {}
Tile::Tile(const tile_type t) { set_type(t); }
Tile::~Tile() {}

const tile_type Tile::get_type() const { return type; }
const vector<entity_id> &Tile::get_entities() const { return entities; }

void Tile::set_type(const tile_type t) { type = t; }
void Tile::add_entity(const entity_id id) { entities.push_back(id); }
void Tile::remove_entity(const entity_id id) {
  for (auto it = entities.begin(); it != entities.end(); it++) {
    if (*it == id) {
      entities.erase(it);
      break;
    }
  }
}
