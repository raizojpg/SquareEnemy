#include "Bullet.h"

Bullet::Bullet(sf::Vector2f position) : DynamicObject{position,{20,10},0,5},
                                        Controllable(), range{150}{
    shape.setFillColor(sf::Color::White);
    std::cout<<"Bullet Created\n";
}

void Bullet::move() {
    shape.move(-speed,0);
    range--;
    if(range == 0){
        wpn->deallocate();
    }
}

void Bullet::possibleAttack(DynamicObject &target) {
    if(wpn != nullptr){
        if(this->getCollisionBox().checkCollision(target.getCollisionBox(),1.0)) {
            std::cout<<"YOU'VE BEEN SHOT"<<std::endl;
            wpn->attack(target);
            wpn->deallocate();
        }
    }
}

void Bullet::addWeapon(Weapon &wpn_) { wpn = &wpn_; }

Bullet::~Bullet() {
    std::cout<<"Bullet destroyed\n";
}
