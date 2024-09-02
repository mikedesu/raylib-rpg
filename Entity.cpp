#include "Entity.h"

void Entity_create(Entity& e, const EntityId e_id, const EntityType t, string n) {
    e.id = e_id;
    e.type = t;
    e.name = n;
    e.light_lvl = 0;
}

void Entity_create(Entity& e, const EntityId e_id, const EntityType t, string n, const int light) {
    e.id = e_id;
    e.type = t;
    e.name = n;
    e.light_lvl = light;
}

void Entity_destroy(Entity& e) { }

const EntityId Entity_get_id(Entity& e) {
    return e.id;
}
const EntityType Entity_get_type(Entity& e) {
    return e.type;
}
const string Entity_get_name(Entity& e) {
    return e.name;
}
const int Entity_get_light_lvl(Entity& e) {
    return e.light_lvl;
}

void Entity_set_name(Entity& e, const string n) {
    e.name = n;
}
void Entity_set_light_lvl(Entity& e, const int light) {
    e.light_lvl = light;
}
