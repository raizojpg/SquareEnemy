#ifndef OOP_STATICPLATFORM_H
#define OOP_STATICPLATFORM_H

#include "Platform.h"

class StaticPlatform : public Platform{
    int alpha;
    int step;
    bool increasing;
public:
    StaticPlatform(sf::Vector2f position_, sf::Vector2f size_, unsigned int durability_) : Platform{position_,size_,durability_},
    alpha{150}, step{1}, increasing{true} {}

    void action() override;

};


#endif //OOP_STATICPLATFORM_H
