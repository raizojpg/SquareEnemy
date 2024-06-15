#include "Error.h"

ObjectError::ObjectError(const std::string &msg) :
        GameError("OBJECT ERROR: " + msg) {}

DynamicObjectError::DynamicObjectError(const std::string &msg) :
        GameError("DYNAMIC OBJECT ERROR: " + msg) {}

EnemyError::EnemyError(const std::string &msg) :
        GameError("ENEMY ERROR: " + msg) {}

WeaponError::WeaponError(const std::string &msg) :
        GameError("WEAPON ERROR: " + msg) {}

LevelError::LevelError(const std::string &msg) :
        GameError("LEVEL ERROR: " + msg) {}
