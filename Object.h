#ifndef OOP_OBJECT_H
#define OOP_OBJECT_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Collision.h"
#include  "Error.h"

std::ostream& operator<< (std::ostream& os, const sf::Vector2f& vec2f);

class Object{
protected:
    sf::Vector2f position;
    sf::Vector2f size;
    sf::RectangleShape shape;
    Collision collisionBox;

    virtual void print(std::ostream& os) const;

public:
    Object();
    Object(sf::Vector2f position_, sf::Vector2f size_);
    Object(const Object& other);
    Object& operator= (const Object& other);

    void setPosition(sf::Vector2f position_);
    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;
    sf::RectangleShape& getShape();
    Collision& getCollisionBox();

    virtual void draw(sf::RenderWindow& window);

    friend std::ostream& operator<< (std::ostream& os, const Object& obj);
    virtual ~Object();

};

#endif //OOP_OBJECT_H
