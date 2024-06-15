#ifndef OOP_ERROR_H
#define OOP_ERROR_H

#include <stdexcept>
#include <string>

class GameError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class ObjectError : public GameError {
public:
    explicit ObjectError(const std::string& msg);
};

class DynamicObjectError : public GameError {
public:
    explicit DynamicObjectError(const std::string& msg);
};

class EnemyError : public GameError {
public:
    explicit EnemyError(const std::string& msg);
};

class WeaponError : public GameError {
public:
    explicit WeaponError(const std::string& msg);
};

class LevelError : public GameError {
public:
    explicit LevelError(const std::string& msg);
};

#endif //OOP_ERROR_H
