#include "Object.h"

std::ostream &operator<<(std::ostream &os, const sf::Vector2f &vec2f) {
    os << "("<<vec2f.x<<":"<<vec2f.y<<") ";
    return os;
}

void Object::print(std::ostream &os) const {
    os << "position = " << position << ", size = " << size;
}

Object::Object() : position{ 0,0 }, size{ 0,0 }{
    std::cout << "Object created NULL " << std::endl;
}

Object::Object(sf::Vector2f position_, sf::Vector2f size_) : position{ position_ }, size{ size_ }{
    if(size.x<0 || size.y<0){
        throw ObjectError("You tried to create an object with negative size");
    }
    std::cout << "Object created " << *this;
    shape = sf::RectangleShape{size};
    shape.setFillColor(sf::Color::Magenta);
    shape.setOrigin(size.x/2,size.y/2);
    shape.setPosition(position);
    collisionBox = Collision{shape};
}

Object::Object(const Object &other) : position{other.position}, size{other.size}, shape{other.shape}{
    collisionBox = Collision{shape};
    std::cout << "Object copied "<< *this;
}

Object &Object::operator=(const Object &other) {
    position = other.position;
    size = other.size;
    shape = other.shape;
    collisionBox = Collision{shape};
    std::cout << "Object operator= "<< *this;
    return *this;
}

void Object::setPosition(const sf::Vector2f position_) { position = position_; shape.setPosition(position); }

sf::Vector2f Object::getPosition() const { return position; }

sf::Vector2f Object::getSize() const { return size; }

sf::RectangleShape &Object::getShape() {return shape;}

Collision &Object::getCollisionBox() {return collisionBox;}

void Object::draw(sf::RenderWindow &window) { window.draw(shape);}

std::ostream &operator<<(std::ostream &os, const Object &obj) {
    obj.print(os);
    os<<std::endl;
    return os;
}

Object::~Object() {
    std::cout << "Object was destroyed " << *this;
}
