#ifndef OOP_GUN_H
#define OOP_GUN_H

#include "Weapon.h"
#include "Bullet.h"

class Gun : public Weapon{
    Bullet* bullet;

    void print(std::ostream& os) const override;

public:
    Gun();
    Gun(int attackDamage_, unsigned int durability_);
    explicit Gun(const Weapon& wpn_);

    [[nodiscard]] Weapon* clone() const override;
    [[nodiscard]] Bullet* getBullet() const;

    void shoot(sf::Vector2f position);
    void draw(sf::RenderWindow& window) override;
    void render() override;

    void deallocate() override;
    ~Gun() override;
};

#endif //OOP_GUN_H
