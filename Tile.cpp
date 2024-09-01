#include "Tile.h"

void Tile_create(Tile& t) {
    t.type = TILE_VOID;
    Tile_set_light_level(t, TILE_DEFAULT_LIGHT_LEVEL);
}

void Tile_create(Tile& t, const TileType type) {
    t.type = type;
    Tile_set_light_level(t, TILE_DEFAULT_LIGHT_LEVEL);
}

void Tile_destroy(Tile& t) { }

const TileType Tile_get_type(Tile& t) {
    return t.type;
}

const vector<EntityId>& Tile_get_entities(Tile& t) {
    return t.entities;
}

const string Tile_get_type_str(Tile& t) {
    string s = "TILE_NONE";
    switch(t.type) {
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

const int Tile_get_light_level(Tile& t) {
    return t.light_level;
}

const int Tile_get_max_light_level(Tile& t) {
    return t.max_light_level;
}

void Tile_set_type(Tile& t, const TileType type) {
    t.type = type;
}

void Tile_add_entity(Tile& t, const EntityId id) {
    t.entities.push_back(id);
}

void Tile_increase_light_level(Tile& t) {
    Tile_increase_light_level_by(t, 1);
}

void Tile_decrease_light_level(Tile& t) {
    Tile_decrease_light_level_by(t, 1);
}

void Tile_remove_entity(Tile& t, const EntityId id) {
    for(auto it = t.entities.begin(); it != t.entities.end(); it++) {
        if(*it == id) {
            t.entities.erase(it);
            break;
        }
    }
}

void Tile_set_light_level(Tile& t, const int level) {
    if(level < TILE_DEFAULT_MIN_LIGHT_LEVEL) {
        t.light_level = TILE_DEFAULT_MIN_LIGHT_LEVEL;
    } else if(level > TILE_DEFAULT_MAX_LIGHT_LEVEL) {
        t.light_level = TILE_DEFAULT_MAX_LIGHT_LEVEL;
    } else {
        t.light_level = level;
    }
    //mPrint("Light level set to: " + to_string(light_level));
}

void Tile_increase_light_level_by(Tile& t, const int level) {
    t.light_level += level;
    if(t.light_level > t.max_light_level) {
        t.light_level = t.max_light_level;
    }
}

void Tile_decrease_light_level_by(Tile& t, const int level) {
    t.light_level -= level;
    if(t.light_level < t.min_light_level) {
        t.light_level = t.min_light_level;
    }
}
