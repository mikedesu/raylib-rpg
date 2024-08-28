#pragma once

#include "EntityType.h"
#include "entity_id.h"
#include <string>

using std::string;

class Entity {

public:
  Entity();
  Entity(entity_id e_id, EntityType t, string n);
  ~Entity();

  void set_name(string n);
  const string get_name() const;

private:
  string name = "Unnamed";
  entity_id id;
  EntityType type;
};
