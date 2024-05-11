#include "Gun.h"

void Gun::print(std::ostream &os) const {
    os << "Gun : attackDamage = " << attackDamage << " and durability = " << durability;
}

Gun::Gun() : Weapon(), bullet{nullptr}{
    std::cout << "Gun created NULL" << std::endl;
}

Gun::Gun(int attackDamage_, unsigned int durability_) :  Weapon{attackDamage_, durability_}, bullet{nullptr} {
    std::cout << "Gun created " << *this;
}

Gun::Gun(const Weapon &wpn_) :  Weapon{wpn_}, bullet{nullptr} {
    std::cout<<"Gun copied "<< *this;
}

Weapon *Gun::clone() const { return new Gun(*this); }

Bullet *Gun::getBullet() const { return bullet; }

void Gun::shoot(const sf::Vector2f position) {
    if(bullet == nullptr){
        bullet = new Bullet(position);
        bullet->addWeapon(*this);
    }
}

void Gun::draw(sf::RenderWindow &window) {
    if(bullet != nullptr){
        bullet->draw(window);
    }
}

void Gun::render() {
    if(bullet != nullptr){
        bullet->move();
    }
}

void Gun::deallocate() {
    delete bullet;
    bullet = nullptr;
}

Gun::~Gun() {
    std::cout<<"Gun destroyed\n";
}
