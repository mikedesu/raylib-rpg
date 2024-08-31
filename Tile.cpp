#include "Tile.h"

Tile::Tile() {
    type = TILE_VOID;
    set_light_level(TILE_DEFAULT_LIGHT_LEVEL);
}

Tile::Tile(const TileType t) {
    type = t;
    set_light_level(TILE_DEFAULT_LIGHT_LEVEL);
}

Tile::~Tile() { }

const TileType Tile::get_type() const {
    return type;
}

const vector<EntityId>& Tile::get_entities() const {
    return entities;
}

const string Tile::get_type_str() const {
    string s = "TILE_NONE";
    switch(type) {
    case TILE_NONE:
        s = "TILE_NONE";
        break;
    case TILE_VOID:
        s = "TILE_VOID";
        break;
    case TILE_FLOOR_BASIC:
        s = "TILE_FLOOR_BASIC";
        break;
    case TILE_FLOOR_STONE:
        s = "TILE_FLOOR_STONE";
        break;
    case TILE_FLOOR_WOOD:
        s = "TILE_FLOOR_WOOD";
        break;
    case TILE_FLOOR_DIRT:
        s = "TILE_FLOOR_DIRT";
        break;
    case TILE_FLOOR_UPSTAIRS:
        s = "TILE_FLOOR_UPSTAIRS";
        break;
    case TILE_FLOOR_DOWNSTAIRS:
        s = "TILE_FLOOR_DOWNSTAIRS";
        break;
    case TILE_COUNT:
        s = "TILE_COUNT";
        break;
    }
    return s;
}

const int Tile::get_light_level() const {
    return light_level;
}

const int Tile::get_max_light_level() const {
    return max_light_level;
}

void Tile::set_type(const TileType t) {
    type = t;
}

void Tile::add_entity(const EntityId id) {
    entities.push_back(id);
}

void Tile::increase_light_level() {
    increase_light_level_by(1);
}

void Tile::decrease_light_level() {
    decrease_light_level_by(1);
}

void Tile::remove_entity(const EntityId id) {
    for(auto it = entities.begin(); it != entities.end(); it++) {
        if(*it == id) {
            entities.erase(it);
            break;
        }
    }
}

void Tile::set_light_level(const int level) {
    if(level < TILE_DEFAULT_MIN_LIGHT_LEVEL) {
        light_level = TILE_DEFAULT_MIN_LIGHT_LEVEL;
    } else if(level > TILE_DEFAULT_MAX_LIGHT_LEVEL) {
        light_level = TILE_DEFAULT_MAX_LIGHT_LEVEL;
    } else {
        light_level = level;
    }
    //mPrint("Light level set to: " + to_string(light_level));
}

void Tile::increase_light_level_by(const int level) {
    light_level += level;
    if(light_level > max_light_level) {
        light_level = max_light_level;
    }
}

void Tile::decrease_light_level_by(const int level) {
    light_level -= level;
    if(light_level < min_light_level) {
        light_level = min_light_level;
    }
}
