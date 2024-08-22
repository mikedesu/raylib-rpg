#include "DungeonEvent.h"

DungeonEvent::DungeonEvent(const entity_id a_id, const DungeonEventType t,
                           const Vector2 loc) {
  type = t;
  actor_id = a_id;
  target = loc;
}
DungeonEvent::~DungeonEvent() {}

const DungeonEventType DungeonEvent::get_type() const { return type; }
const entity_id DungeonEvent::get_actor_id() const { return actor_id; }
const Vector2 DungeonEvent::get_target() const { return target; }
