#include "Arms.h"

void Arms::print(std::ostream &os) const {
    Weapon::print(os);
    os<<" fists: "<<fistL<<" "<<fistR;
}


Arms::Arms() : fistL{} , fistR() {std::cout << "Arms created NULL" << std::endl;}

Arms::Arms(int attackDamage_, unsigned int durability_) : Weapon{attackDamage_,durability_},
                                                          fistL{position,{20,40}},
                                                          fistR{position,{20,40}}{
    fistL.getShape().setFillColor(sf::Color::Green);
    fistR.getShape().setFillColor(sf::Color::Green);
    std::cout << "Arms created " << *this;
}

Weapon *Arms::clone() const {return new Arms(*this);}

void Arms::possible_impact(DynamicObject &target) {
    if(fistL.getCollisionBox().checkCollision(target.getCollisionBox(),1.0)){
        attack(target);
    }
    if(fistR.getCollisionBox().checkCollision(target.getCollisionBox(),1.0)){
        attack(target);
    }
}

void Arms::draw(sf::RenderWindow &window) {
    fistL.draw(window);
    fistR.draw(window);
}

void Arms::render(const DynamicObject &parent) {
    fistR.setPosition({parent.getPosition().x+parent.getSize().x/2,parent.getPosition().y});
    fistL.setPosition({parent.getPosition().x-parent.getSize().x/2,parent.getPosition().y});
}

Arms::~Arms() {
    std::cout<<"Arms destroyed"<<std::endl;
}


