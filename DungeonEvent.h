#pragma once

#include "entity_id.h"
#include "raylib.h"

#include <string>

using std::string;

typedef enum {
  EVENT_NONE = 0,
  EVENT_ENTITY_MOVE_SUCCESS,
  EVENT_ENTITY_MOVE_FAIL,
  EVENT_ENTITY_MOVE_WAIT,
} DungeonEventType;

class DungeonEvent {

public:
  // DungeonEvent(const DungeonEventType t, const entity_id a_id) {
  DungeonEvent(const entity_id a_id, const DungeonEventType t,
               const Vector2 loc);
  ~DungeonEvent();

  const DungeonEventType get_type() const;
  const entity_id get_actor_id() const;
  const Vector2 get_target() const;

  const string get_message() const;

private:
  DungeonEventType type;
  entity_id actor_id;
  Vector2 target;
  string message;
};
