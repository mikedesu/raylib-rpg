#pragma once

#include "EntityId.h"
#include "EntityType.h"
#include <string>

using std::string;

class Entity {

private:
    const EntityId id;
    const EntityType type;
    string name = "Unnamed";
    int light_lvl = 0;

public:
    Entity(const EntityId e_id, const EntityType t, string n)
        : id(e_id)
        , type(t)
        , name(n) { }
    Entity(const EntityId e_id, const EntityType t, string n, const int light)
        : id(e_id)
        , type(t)
        , name(n)
        , light_lvl(light) { }
    ~Entity() { }

    const EntityId get_id() const {
        return id;
    }
    const EntityType get_type() const {
        return type;
    }
    const string get_name() const {
        return name;
    }
    const int get_light_lvl() const {
        return light_lvl;
    }

    void set_name(const string n) {
        name = n;
    }
    void set_light_lvl(const int light) {
        light_lvl = light;
    }
};
