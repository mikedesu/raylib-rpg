#pragma once

#include "EntityType.h"
#include "entity_id.h"
#include <string>

using std::string;

class Entity {

public:
  Entity(const entity_id e_id, EntityType t, string n)
      : id(e_id), type(t), name(n) {}
  ~Entity() {}

  const entity_id get_id() const { return id; }
  const EntityType get_type() const { return type; }
  const string get_name() const { return name; }

  void set_name(const string n) { name = n; }

private:
  const entity_id id;
  const EntityType type;
  string name = "Unnamed";
};
