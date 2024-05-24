#ifndef OOP_WEAPON_H
#define OOP_WEAPON_H

#include <chrono>
#include <thread>
#include "DynamicObject.h"

class Weapon {
protected:
    int attackDamage;
    unsigned int durability;
    sf::Vector2f position;
    bool canAttack;
    std::chrono::steady_clock::time_point lastAttackTime;
    const std::chrono::steady_clock::duration attackCooldown;

    virtual void print(std::ostream& os) const;

    Weapon();
    Weapon(int attackDamage_, unsigned int durability_);
    Weapon(const Weapon& other);
    Weapon& operator= (const Weapon& other);

public:

    [[nodiscard]] virtual Weapon* clone() const = 0;

//    int getAttackDamage() const { return attackDamage; }
//    void setAttackDamage(int attackDamage_) { attackDamage = attackDamage_; }
//    unsigned int getDurability() const { return durability; }
//    void setDurability(unsigned int durability_) { durability = durability_; }

    void attack(DynamicObject& target);

    void setPosition(sf::Vector2f position_);

    virtual void possible_impact(DynamicObject& target) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void render(const DynamicObject& parent) = 0;

    friend std::ostream& operator<< (std::ostream& os, const Weapon& obj);

    virtual ~Weapon();
};

#endif //OOP_WEAPON_H
