#ifndef OOP_SPEAR_H
#define OOP_SPEAR_H

#include "Weapon.h"

class Spear : public Weapon{
private:
    Object spear;

    void print(std::ostream& os) const override;
public:
    Spear();

    Spear(int attackDamage_, unsigned int durability_);
    Spear(const Spear& other) = default;
    Spear& operator=(const Spear& other) = default;

    Weapon* clone() const override;

    void possible_impact(DynamicObject &target) override;

    void draw(sf::RenderWindow &window) override;

    void render(const DynamicObject &parent) override;

    ~Spear() override;

};


#endif //OOP_SPEAR_H
