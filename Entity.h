#pragma once

#include "EntityId.h"
#include "EntityType.h"
#include <string>

using std::string;

typedef struct {
    EntityId id;
    EntityType type;
    string name = "Unnamed";
    int light_lvl = 0;
} Entity;

void Entity_create(Entity& e, const EntityId e_id, const EntityType t, string n);
void Entity_create(Entity& e, const EntityId e_id, const EntityType t, string n, const int light);
void Entity_destroy(Entity& e);
const EntityId Entity_get_id(Entity& e);
const EntityType Entity_get_type(Entity& e);
const string Entity_get_name(Entity& e);
const int Entity_get_light_lvl(Entity& e);
void Entity_set_name(Entity& e, const string n);
void Entity_set_light_lvl(Entity& e, const int light);
