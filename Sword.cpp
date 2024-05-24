#include "Sword.h"

void Sword::print(std::ostream &os) const {
    Weapon::print(os);
    os <<" blade: "<< blade;
}

Sword::Sword() : blade{}, angle{0}, swing{0} {std::cout << "Blade created NULL" << std::endl;}

Sword::Sword(int attackDamage_, unsigned int durability_) : Weapon{attackDamage_,durability_},
                                                            blade{position,{20,20}},angle{2},swing{2}{
    blade.getShape().setFillColor(sf::Color::Magenta);
    mask.setFillColor(sf::Color::White);
    mask.setSize({blade.getSize().x/2,blade.getSize().y*4});
    mask.setOrigin(mask.getSize().x,mask.getSize().y);
    mask.setPosition(position);
    mask.setRotation(200);
    std::cout << "Blade created " << *this;
}

Weapon *Sword::clone() const {return new Sword(*this);}

void Sword::possible_impact(DynamicObject &target) {
    if(blade.getCollisionBox().checkCollision(target.getCollisionBox(),1.0)){
        attack(target);
    }
}

void Sword::draw(sf::RenderWindow &window) {
    //blade.draw(window);
    window.draw(mask);

}

void Sword::render(const DynamicObject &parent) {
    mask.setPosition(parent.getPosition().x-parent.getSize().x/2,parent.getPosition().y);
    blade.setPosition({mask.getPosition().x-mask.getSize().y+mask.getSize().x*2,mask.getPosition().y-swing*mask.getSize().x*2});
    if(mask.getRotation()>270){swing = 2;}
    if(mask.getRotation()<270){swing = -2;}
    if(mask.getRotation()>350){angle = angle*(-1);}
    if(mask.getRotation()<180){angle = angle*(-1);}
    mask.rotate(angle);
}

Sword::~Sword() {
    std::cout<<"Sword destroyed"<<std::endl;
}

