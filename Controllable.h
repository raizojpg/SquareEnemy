#ifndef OOP_CONTROLLABLE_H
#define OOP_CONTROLLABLE_H

#include "Weapon.h"

class Controllable{
protected:
    Weapon* wpn;
public:
    Controllable();
    //explicit Controllable(Weapon& wpn_) { wpn = new Weapon(wpn_);}
    Controllable(const Controllable& other);
    Controllable& operator= (const Controllable& other);
    Controllable& operator= (Controllable* other);

    friend void swap(Controllable &c1, Controllable &c2);

    [[nodiscard]] Weapon* getWeapon() const;
    virtual void addWeapon(Weapon& wpn_) = 0;
    virtual void possibleAttack(DynamicObject& obj) = 0;
    virtual void move() = 0;

    virtual ~Controllable() = default;
};
#endif //OOP_CONTROLLABLE_H
