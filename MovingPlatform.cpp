#include "MovingPlatform.h"

MovingPlatform::MovingPlatform(sf::Vector2f position_, sf::Vector2f size_, unsigned int durability_, float speed_,
                               int maxSteps_, bool horizontal_)
        : Platform{position_,size_,durability_}, speed{speed_}, steps{0}, maxSteps{maxSteps_}, horizontal{horizontal_}{}

void MovingPlatform::move() {
    if(horizontal){
        if(steps / (maxSteps / 2) == 0){
            shape.move(+speed,0);
            if(steps==maxSteps-1){steps=(-1)*maxSteps;}
        }
        else if(steps / (maxSteps / 2) == 1){
            shape.move(-speed,0);
        }
        else{
            steps = -1;
        }
    }
    else{
        if(steps / (maxSteps / 2) == 0){
            shape.move(0,+speed);
            if(steps==maxSteps-1){steps=(-1)*maxSteps;}
        }
        else if(steps / (maxSteps / 2) == 1){
            shape.move(0,-speed);
        }
        else{
            steps = -1;
        }
    }
    steps++;
    position.x = shape.getPosition().x;
    position.y = shape.getPosition().y;
}
