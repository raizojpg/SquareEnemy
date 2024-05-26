#include "Spear.h"

void Spear::print(std::ostream &os) const {
    Weapon::print(os);
    os <<" blade: "<< spear;
}

Spear::Spear() : Weapon(), spear{}{
    std::cout << "Spear created NULL" << std::endl;
}

Spear::Spear(int attackDamage_, unsigned int durability_) : Weapon{attackDamage_,durability_},
                                                            spear{position,{140,10}} {
    spear.getShape().setFillColor(sf::Color::White);
    spear.getShape().setOrigin({spear.getSize().x/2,spear.getSize().y/2});
    std::cout << "Spear created " << *this;
}

Weapon *Spear::clone() const {return new Spear(*this);}

void Spear::possible_impact(DynamicObject &target) {
    if(spear.getCollisionBox().checkCollision(target.getCollisionBox(),1.0)){
        attack(target);
    }
}

void Spear::draw(sf::RenderWindow &window) {
    spear.draw(window);
}

void Spear::render(const DynamicObject &parent) {
    spear.setPosition({parent.getPosition().x+parent.getSize().x/2, parent.getPosition().y} );
}

Spear::~Spear() {
    std::cout<<"Spear destroyed"<<std::endl;
}




