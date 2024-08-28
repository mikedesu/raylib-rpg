#pragma once

#include "EntityType.h"
#include "entity_id.h"
#include <string>

using std::string;

class Entity {

private:
  const entity_id id;
  const EntityType type;
  string name = "Unnamed";
  int light_lvl = 0;

public:
  Entity(const entity_id e_id, const EntityType t, string n)
      : id(e_id), type(t), name(n) {}
  Entity(const entity_id e_id, const EntityType t, string n, const int light)
      : id(e_id), type(t), name(n), light_lvl(light) {}
  ~Entity() {}

  const entity_id get_id() const { return id; }
  const EntityType get_type() const { return type; }
  const string get_name() const { return name; }
  const int get_light_lvl() const { return light_lvl; }

  void set_name(const string n) { name = n; }
  void set_light_lvl(const int light) { light_lvl = light; }
};
