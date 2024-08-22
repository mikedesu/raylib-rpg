#pragma once

#include <string>

using std::string;

class Entity {

public:
  Entity();
  Entity(string n);
  ~Entity();

  void set_name(string n);
  const string get_name() const;

private:
  string name = "Unnamed";
};
