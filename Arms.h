#ifndef OOP_ARMS_H
#define OOP_ARMS_H

#include "Weapon.h"

class Arms : public Weapon {
private:
    Object fistL;
    Object fistR;

    void print(std::ostream& os) const override;

public:
    Arms();
    Arms(int attackDamage_, unsigned int durability_);
    Arms(const Arms& other) = default;
    Arms& operator= (const Arms& other) = default;

    [[nodiscard]] Weapon* clone() const override;

    void possible_impact(DynamicObject& target) override;
    void draw(sf::RenderWindow& window) override;
    void render(const DynamicObject& parent) override;

    ~Arms() override;
};

#endif //OOP_ARMS_H
