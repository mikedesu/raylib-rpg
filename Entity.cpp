#include "Entity.h"

Entity::Entity() {
  // Constructor
  name = "unnamed";
  id = -1;
  type = ENTITY_NONE;
}

Entity::Entity(entity_id e_id, EntityType t, string n) {
  // Constructor
  name = n;
  id = e_id;
  type = t;
}

Entity::~Entity() {
  // Destructor
}

void Entity::set_name(string n) { name = n; }
const string Entity::get_name() const { return name; }
