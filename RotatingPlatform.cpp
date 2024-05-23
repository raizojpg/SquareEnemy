#include "RotatingPlatform.h"

RotatingPlatform::RotatingPlatform(sf::Vector2f position_, sf::Vector2f size_, unsigned int durability_) : Platform{position_,size_,durability_}, angle{5}{
    mask.setFillColor(sf::Color::Magenta);
    mask.setSize({size.x,size.y/5});
    mask.setOrigin(mask.getSize().x/2,mask.getSize().y/2);
    mask.setPosition(position);
}

void RotatingPlatform::action() {
    mask.rotate(angle);
}

void RotatingPlatform::draw(sf::RenderWindow &window) {
    window.draw(mask);
}
