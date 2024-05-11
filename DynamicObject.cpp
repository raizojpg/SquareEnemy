#include "DynamicObject.h"

void DynamicObject::print(std::ostream &os) const {
    Object::print(os);
    os << ", hp = " << hp << " , speed = "<< speed;
}

DynamicObject::DynamicObject() : hp{0}, speed{0} {
    std::cout << "DynamicObject created NULL " << std::endl;
}

DynamicObject::DynamicObject(sf::Vector2f position_, sf::Vector2f size_, unsigned int hp_, float speed_) :
        Object{position_, size_}, hp{hp_}, speed{speed_} {
    if(speed<0){
        throw DynamicObjectError("Speed must be positive in order to move in this universe");
    }
    std::cout << "DynamicObject created " << std::endl;
    shape.setFillColor(sf::Color::Cyan);
}

DynamicObject::DynamicObject(const DynamicObject &other) : Object{other}, hp{other.hp}, speed{other.speed} {
    std::cout << "DynamicObject copied " << std::endl;
}

DynamicObject &DynamicObject::operator=(const DynamicObject &other) {
    Object::operator=(other);
    hp = other.hp;
    speed = other.speed;
    std::cout << " DynamicObject operator= ";
    return *this;
}

void DynamicObject::showHP(sf::RenderWindow *window) {
    if(window != nullptr) {
        hp_text.getText().setString(std::to_string(hp));
        hp_text.getText().setOrigin(this->getShape().getOrigin());
        hp_text.getText().setPosition(this->position);
        hp_text.draw(*window);
    }
}

unsigned int DynamicObject::getHp() const {return hp;}

void DynamicObject::setHp(unsigned int hp_) {hp = hp_;}

bool DynamicObject::checkHP() const {
    return hp==0;
}

DynamicObject::~DynamicObject() {
    std::cout << "DynamicObject destroyed " << std::endl;
}
