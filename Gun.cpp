#include "Gun.h"

void Gun::print(std::ostream &os) const {
    Weapon::print(os);
    os << " bullet: " << bullet;
}

Gun::Gun() : Weapon(), bullet{}, range{0}{
    std::cout << "Gun created NULL" << std::endl;
}

Gun::Gun(int attackDamage_, unsigned int durability_) :  Weapon{attackDamage_, durability_},
    bullet{position,{20,10},0,5}, range{150} {
    bullet.getShape().setFillColor(sf::Color::White);
    std::cout << "Gun created " << *this;
}

Weapon *Gun::clone() const { return new Gun(*this); }

void Gun::possible_impact(DynamicObject &target) {
    if(bullet.getCollisionBox().checkCollision(target.getCollisionBox(),1.0)){
        attack(target);
        bullet.setPosition(position);
    }
}

void Gun::draw(sf::RenderWindow &window) {
    bullet.draw(window);
}

void Gun::render(const DynamicObject& parent) {
    bullet.getShape().move(-bullet.getSpeed(),0);
    range--;
    if(range == 0){
        bullet.setPosition(parent.getPosition());
        range = 150;
    }
}

Gun::~Gun() {
    std::cout<<"Gun destroyed\n";
}
