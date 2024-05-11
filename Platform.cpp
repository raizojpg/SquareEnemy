#include "Platform.h"

void Platform::print(std::ostream &os) const {
    Object::print(os);
    os << ", durability = "<<durability;
}

Platform::Platform() :  durability{ 0 } {
    std::cout << "Platform created NULL " << std::endl;
}

Platform::Platform(sf::Vector2f position_, sf::Vector2f size_, unsigned int durability_) :
        Object{position_, size_}, durability{ durability_ }{
    std::cout << "Platform created " << *this;
}

Platform::Platform(const Platform &other) : Object{other}, durability{other.durability}{
    std::cout << "Platform copied " << *this;
}

Platform &Platform::operator=(const Platform &other) {
    Object::operator=(other);
    durability = other.durability;
    std::cout << "Platform operator= "<< *this;
    return *this;
}

Platform::~Platform() {
    std::cout << "Platform was destroyed " << *this;
}
