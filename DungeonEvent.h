#pragma once

#include "EntityId.h"
#include "raylib.h"

#include <string>

// using std::string;
using namespace std;

typedef enum {
    EVENT_NONE = 0,
    EVENT_ENTITY_MOVE_SUCCESS,
    EVENT_ENTITY_MOVE_FAIL,
    EVENT_ENTITY_MOVE_WAIT,
} DungeonEventType;

class DungeonEvent {

private:
    EntityId actor_id;
    DungeonEventType type;
    Vector2 target;

public:
    DungeonEvent(const EntityId a_id, const DungeonEventType t, const Vector2 loc)
        : actor_id(a_id)
        , type(t)
        , target(loc) { }
    ~DungeonEvent() { }

    const DungeonEventType get_type() const {
        return type;
    }
    const EntityId get_actor_id() const {
        return actor_id;
    }
    const Vector2 get_target() const {
        return target;
    }

    const string get_message() const {
        string s = "";
        switch(type) {
        case EVENT_ENTITY_MOVE_SUCCESS:
            s = "moved to";
            break;
        case EVENT_ENTITY_MOVE_FAIL:
            s = "cannot move to";
            break;
        case EVENT_ENTITY_MOVE_WAIT:
            s = "is waiting at";
            break;
        default:
            s = "did an unknown action at";
            break;
        }
        return s;
    }
};
