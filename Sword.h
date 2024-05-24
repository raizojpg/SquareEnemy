#ifndef OOP_SWORD_H
#define OOP_SWORD_H

#include "Weapon.h"

class Sword : public Weapon {
private:
    Object blade;
    float angle;
    float swing;
    sf::RectangleShape mask;

    void print(std::ostream& os) const override;

public:
    Sword();
    Sword(int attackDamage_, unsigned int durability_);
    Sword(const Sword& other) = default;
    Sword& operator=(const Sword& other) = default;

    [[nodiscard]] Weapon* clone() const override;

    void possible_impact(DynamicObject& target) override;
    void draw(sf::RenderWindow& window) override;
    void render(const DynamicObject& parent) override;

    ~Sword() override;
};

#endif //OOP_SWORD_H
