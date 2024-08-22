#include "Entity.h"

Entity::Entity() {
  // Constructor
}

Entity::Entity(string n) {
  // Constructor
  name = n;
}

Entity::~Entity() {
  // Destructor
}

void Entity::set_name(string n) { name = n; }
const string Entity::get_name() const { return name; }
