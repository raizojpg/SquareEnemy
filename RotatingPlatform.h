#ifndef OOP_ROTATINGPLATFORM_H
#define OOP_ROTATINGPLATFORM_H

#include "Platform.h"

class RotatingPlatform : public Platform{
    float angle;
    sf::RectangleShape mask;
public:
    RotatingPlatform(sf::Vector2f position_, sf::Vector2f size_, unsigned int durability_);

    void action() override;

    void draw(sf::RenderWindow& window) override;

};

#endif //OOP_ROTATINGPLATFORM_H
