#ifndef OOP_MOVINGPLATFORM_H
#define OOP_MOVINGPLATFORM_H

#include "Platform.h"

class MovingPlatform : public Platform  {
    float speed;
    int steps;
    int maxSteps;
    bool horizontal;
public:
    MovingPlatform(sf::Vector2f position_, sf::Vector2f size_, unsigned int durability_, float speed_ ,int maxSteps_, bool horizontal_);

    void action() override;

};

#endif //OOP_MOVINGPLATFORM_H
