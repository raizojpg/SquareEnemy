#ifndef OOP_BULLET_H
#define OOP_BULLET_H

#include "DynamicObject.h"
#include "Controllable.h"

class Bullet : public DynamicObject, public Controllable{
private:
    int range;
public:
    explicit Bullet(sf::Vector2f position);
    void move() override;
    void possibleAttack(DynamicObject& target) override;
    void addWeapon(Weapon& wpn_) override;
    ~Bullet() override;
};

#endif //OOP_BULLET_H
