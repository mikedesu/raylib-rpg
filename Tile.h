#pragma once

#include "EntityId.h"
#include "Sprite.h"
#include "TileLightDefaults.h"
#include "TileType.h"
#include "mPrint.h"

#include <memory>
#include <string>
#include <vector>

using namespace std;

class Tile {

private:
    TileType type = TILE_VOID;
    vector<EntityId> entities;
    int light_level = TILE_DEFAULT_LIGHT_LEVEL;
    const float light_incr = TILE_DEFAULT_LIGHT_INCR;
    const int max_light_level = TILE_DEFAULT_MAX_LIGHT_LEVEL;
    const int min_light_level = TILE_DEFAULT_MIN_LIGHT_LEVEL;

public:
    Tile();

    Tile(const TileType t);

    ~Tile();

    const TileType get_type() const;

    const vector<EntityId>& get_entities() const;

    const string get_type_str() const;

    const int get_light_level() const;

    const int get_max_light_level() const;

    void set_type(const TileType t);

    void add_entity(const EntityId id);

    void increase_light_level();

    void decrease_light_level();

    void remove_entity(const EntityId id);

    void set_light_level(const int level);

    void increase_light_level_by(const int level);
    void decrease_light_level_by(const int level);
};
