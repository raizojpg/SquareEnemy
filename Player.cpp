#include "Player.h"

Player::Player(sf::Vector2f position_, sf::Vector2f size_, int hp_, float speed_) :
        DynamicObject{position_, size_, hp_, speed_}, Controllable() {
    std::cout << "Player created " << std::endl;
    shape.setFillColor(sf::Color::Green);
}

void Player::move() {
    float relative_speed = speed;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
        relative_speed = speed*1.5f;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)){
        shape.move(-relative_speed,0);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){
        shape.move(relative_speed,0);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)){
        shape.move(0,-relative_speed);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)){
        shape.move(0,relative_speed);
    }
    position.x = shape.getPosition().x;
    position.y = shape.getPosition().y;
}

void Player::drag(const sf::RenderWindow& window) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        shape.setPosition(float(mousePosition.x), float(mousePosition.y));
        position.x = (float(mousePosition.x));
        position.y = (float(mousePosition.y));
    }
}

void Player::addWeapon(Weapon &wpn_) {
    wpn = wpn_.clone();
    wpn->setPosition(position);
}

void Player::possibleAttack(DynamicObject &target) {
    if(wpn != nullptr){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)){
            wpn->possible_impact(target);
        }
    }
}

void Player::draw(sf::RenderWindow &window) {
    window.draw(shape);
    if(wpn != nullptr) {
        wpn->draw(window);
    }
}

Player &Player::getInstance(sf::Vector2f position_, sf::Vector2f size_, int hp_, float speed_) {
    static Player instance{position_,size_,hp_,speed_};
    return instance;
}

Player::~Player() {
    std::cout << "Player destroyed " << std::endl;
}

