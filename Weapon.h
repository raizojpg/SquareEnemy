#ifndef OOP_WEAPON_H
#define OOP_WEAPON_H

#include <chrono>
#include <thread>
#include "DynamicObject.h"

class Weapon {
protected:
    int attackDamage;
    unsigned int durability;
    bool canAttack;
    std::chrono::steady_clock::time_point lastAttackTime;
    const std::chrono::steady_clock::duration attackCooldown;

    virtual void print(std::ostream& os) const;

public:
    Weapon();
    Weapon(int attackDamage_, unsigned int durability_);
    Weapon(const Weapon& other);
    Weapon& operator= (const Weapon& other);

    [[nodiscard]] virtual Weapon* clone() const;

//    int getAttackDamage() const { return attackDamage; }
//    void setAttackDamage(int attackDamage_) { attackDamage = attackDamage_; }
//    unsigned int getDurability() const { return durability; }
//    void setDurability(unsigned int durability_) { durability = durability_; }

    void attack(DynamicObject& target);

    virtual void draw(sf::RenderWindow& window);
    virtual void render();
    virtual void deallocate();

    friend std::ostream& operator<< (std::ostream& os, const Weapon& obj);

    virtual ~Weapon();
};

#endif //OOP_WEAPON_H
