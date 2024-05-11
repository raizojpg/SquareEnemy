#include "Collision.h"

Collision::Collision() : shape{nullptr}{}

Collision::Collision(sf::RectangleShape &shape_) : shape{&shape_} {}

bool Collision::checkCollision(Collision &other, float resistence) {
    sf::Vector2f thisPosition = getPosition();
    sf::Vector2f thisHalfSize = getHalfSize();
    sf::Vector2f otherPosition = other.getPosition();
    sf::Vector2f otherHalfSize = other.getHalfSize();
    float dx = abs(thisPosition.x - otherPosition.x) - (thisHalfSize.x + otherHalfSize.x);
    float dy = abs(thisPosition.y - otherPosition.y) - (thisHalfSize.y + otherHalfSize.y);
    if (dx<0 && dy<0){
        resistence = std::min(std::max(0.0f,resistence),1.0f);
        if(dx>dy){
            if(thisPosition.x - otherPosition.x < 0){
                move(dx * (1-resistence),0);
                other.move(-dx * resistence,0);
            }
            else{
                move(-dx * (1-resistence),0);
                other.move(dx * resistence,0);
            }
        }
        else{
            if(thisPosition.y - otherPosition.y < 0){
                move(0,dy * (1-resistence));
                other.move(0,-dy * resistence);
            }
            else {
                move(0, -dy * (1 - resistence));
                other.move(0, dy * resistence);
            }
        }
        return true;
    }
    return false;
}

sf::Vector2f Collision::getPosition() { return (*shape).getPosition();}

sf::Vector2f Collision::getHalfSize() { return (*shape).getSize()/float(2);}

void Collision::move(float dx, float dy) { shape->move(dx,dy);}

template<typename Type>
Type Collision::abs(Type x) {return x>=0 ? x : -x;}
