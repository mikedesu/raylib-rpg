#pragma once

#include "Sprite.h"
#include "entity_id.h"
#include "mPrint.h"

#include <memory>
#include <string>
#include <vector>

using namespace std;

#define TILE_DEFAULT_LIGHT_LEVEL 50
#define TILE_DEFAULT_MAX_LIGHT_LEVEL 100
#define TILE_DEFAULT_MIN_LIGHT_LEVEL 0
#define TILE_DEFAULT_LIGHT_INCR (1.0f / TILE_DEFAULT_MAX_LIGHT_LEVEL)

typedef enum {
    TILE_NONE,
    TILE_VOID,
    TILE_FLOOR_BASIC,
    TILE_FLOOR_STONE,
    TILE_FLOOR_WOOD,
    TILE_FLOOR_DIRT,
    TILE_FLOOR_UPSTAIRS,
    TILE_FLOOR_DOWNSTAIRS,
    TILE_COUNT

} tile_type;

class Tile {

private:
    tile_type type = TILE_VOID;
    vector<entity_id> entities;
    int light_level = TILE_DEFAULT_LIGHT_LEVEL;
    const float light_incr = TILE_DEFAULT_LIGHT_INCR;
    const int max_light_level = TILE_DEFAULT_MAX_LIGHT_LEVEL;
    const int min_light_level = TILE_DEFAULT_MIN_LIGHT_LEVEL;

public:
    Tile()
        : type(TILE_VOID) {
        set_light_level(TILE_DEFAULT_LIGHT_LEVEL);
    }

    Tile(const tile_type t)
        : type(t) {
        set_light_level(TILE_DEFAULT_LIGHT_LEVEL);
    }
    ~Tile() { }

    const tile_type get_type() const {
        return type;
    }
    const vector<entity_id>& get_entities() const {
        return entities;
    }
    const string get_type_str() const {
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
    const int get_light_level() const {
        return light_level;
    }
    const int get_max_light_level() const {
        return max_light_level;
    }
    void set_type(const tile_type t) {
        type = t;
    }
    void add_entity(const entity_id id) {
        entities.push_back(id);
    }
    void increase_light_level() {
        increase_light_level_by(1);
    }
    void decrease_light_level() {
        decrease_light_level_by(1);
    }

    void remove_entity(const entity_id id) {
        for(auto it = entities.begin(); it != entities.end(); it++) {
            if(*it == id) {
                entities.erase(it);
                break;
            }
        }
    }

    void set_light_level(const int level) {
        if(level < TILE_DEFAULT_MIN_LIGHT_LEVEL) {
            light_level = TILE_DEFAULT_MIN_LIGHT_LEVEL;
        } else if(level > TILE_DEFAULT_MAX_LIGHT_LEVEL) {
            light_level = TILE_DEFAULT_MAX_LIGHT_LEVEL;
        } else {
            light_level = level;
        }
        //mPrint("Light level set to: " + to_string(light_level));
    }

    void increase_light_level_by(const int level) {
        light_level += level;
        if(light_level > max_light_level) {
            light_level = max_light_level;
        }
    }

    void decrease_light_level_by(const int level) {
        light_level -= level;
        if(light_level < min_light_level) {
            light_level = min_light_level;
        }
    }
};
