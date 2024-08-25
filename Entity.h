#pragma once

#include "entity_id.h"
#include "entity_type.h"
#include <string>

using std::string;

class Entity {

public:
  Entity();
  Entity(entity_id e_id, entity_type t, string n);
  ~Entity();

  void set_name(string n);
  const string get_name() const;

private:
  string name = "Unnamed";
  entity_id id;
  entity_type type;
};
