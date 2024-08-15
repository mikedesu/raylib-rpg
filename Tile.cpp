#include "Tile.h"

Tile::Tile() {}
Tile::Tile(tile_type t) { set_type(t); }
Tile::~Tile() {}
void Tile::set_type(tile_type t) { type = t; }
const tile_type Tile::get_type() const { return type; }

void Tile::add_entity(entity_id id) { entities.push_back(id); }
void Tile::remove_entity(entity_id id) {
  for (auto it = entities.begin(); it != entities.end(); it++) {
    if (*it == id) {
      entities.erase(it);
      break;
    }
  }
}
const vector<entity_id> &Tile::get_entities() const { return entities; }

void Tile::move_entity_to_tile(entity_id id, Tile &tile) {
  remove_entity(id);
  tile.add_entity(id);
}
