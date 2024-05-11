#include "Weapon.h"

void Weapon::print(std::ostream &os) const {
    os << "Weapon : attackDamage = " << attackDamage << " and durability = " << durability;
}

Weapon::Weapon() : attackDamage{0}, durability{0}, canAttack{false}, attackCooldown{std::chrono::seconds(1)} {
    std::cout << "Weapon created NULL" << std::endl;
}

Weapon::Weapon(int attackDamage_, unsigned int durability_) : attackDamage{attackDamage_}, durability{durability_},
                                                              canAttack{true}, attackCooldown{std::chrono::seconds(1)} {
    if(attackDamage<0){
        throw WeaponError("Your attackDamage is negative, your weapon will heall instead of hraming :)");
    }
    std::cout << "Weapon created " << *this;
}

Weapon::Weapon(const Weapon &other) : attackDamage{other.attackDamage}, durability{other.durability},
                                      canAttack{other.canAttack}, attackCooldown{std::chrono::seconds(1)}{
    std::cout << "Weapon copied "<< *this;
}

Weapon &Weapon::operator=(const Weapon &other) {
    attackDamage = other.attackDamage;
    durability = other.durability;
    canAttack = other.canAttack;
    std::cout << "Weapon operator= " << *this;
    return *this;
}

Weapon *Weapon::clone() const { return new Weapon(*this); }

void Weapon::attack(DynamicObject &target) {
    if(canAttack) {
        canAttack = false;
        target.setHp(target.getHp() - this->attackDamage);
        std::cout<<target.getHp()<<std::endl;
        std::cout << "ATTACK" << std::endl;

        lastAttackTime = std::chrono::steady_clock::now();
        std::thread cooldownTimer([this]() {
            std::this_thread::sleep_for(attackCooldown);
            canAttack = true;
            std::cout<<"COOLDOWN ENDED\n";
        });
        cooldownTimer.detach();
    }
    else{
        std::cout << "Wait for cooldown" << std::endl;
    }
}

void Weapon::draw(sf::RenderWindow &window) {
    Object obj;
    obj.draw(window);
}

void Weapon::render() {}

void Weapon::deallocate() {}

std::ostream &operator<<(std::ostream &os, const Weapon &obj) {
    obj.print(os);
    os<<std::endl;
    return os;
}

Weapon::~Weapon() {
    std::cout << "Weapon was destroyed" << std::endl;
}
