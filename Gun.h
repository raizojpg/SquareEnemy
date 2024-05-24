#ifndef OOP_GUN_H
#define OOP_GUN_H

#include "Weapon.h"

class Gun : public Weapon{
    DynamicObject bullet;
    int range;

    void print(std::ostream& os) const override;

public:
    Gun();
    Gun(int attackDamage_, unsigned int durability_);
    Gun(const Gun& wpn_) = default;
    Gun& operator=(const Gun& other) = default;

    [[nodiscard]] Weapon* clone() const override;
    //DynamicObject& getBullet();

    void possible_impact(DynamicObject& target) override;
    void draw(sf::RenderWindow& window) override;
    void render(const DynamicObject& parent) override;

    ~Gun() override;
};

#endif //OOP_GUN_H
