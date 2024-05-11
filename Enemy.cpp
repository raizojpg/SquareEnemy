#include "Enemy.h"

int Enemy::counter = 0;
Text Enemy::ct_text{"Enemies remaining : ", {-50,-100}};

Enemy::Enemy(sf::Vector2f position_, sf::Vector2f size_, unsigned int hp_, float speed_, int maxSteps_) :
        DynamicObject{position_, size_, hp_, speed_}, Controllable(), steps{0}, maxSteps{maxSteps_} {
    if(maxSteps < 0){
        throw EnemyError("Your enemy must have a positive number of maxSteps in order to move");
    }
    std::cout << "Enemy created " << std::endl;
    shape.setFillColor(sf::Color::Red);
    counter++;
}

Enemy::Enemy(const Enemy &other) : DynamicObject{other}, Controllable{other}, steps{0}, maxSteps{other.maxSteps} {
    std::cout << "Enemy copied " << std::endl;
    counter++;
}

Enemy &Enemy::operator=(const Enemy &other) {
    DynamicObject::operator=(other);
    Controllable::operator=(other);
    maxSteps = other.maxSteps;
    std::cout << " Enemy operator= ";
    return *this;
}

void Enemy::updateCounter(sf::RenderWindow &window) {
    ct_text.getText().setString("Enemies remaining : " + std::to_string(counter));
    ct_text.draw(window);
}

int Enemy::getCounter() { return counter;}

void Enemy::move() {
    if(steps / (maxSteps / 4) == 0){
        shape.move(+speed,-speed/4);
        if(steps==maxSteps-1){steps=(-1)*maxSteps;}
    }
    else if(steps / (maxSteps / 4) == 1){
        shape.move(+speed,+speed/4);
    }
    else if(steps / (maxSteps / 4) == 2){
        shape.move(-speed,+speed/4);
    }
    else if(steps / (maxSteps / 4) == 3){
        shape.move(-speed,-speed/4);
    }
    else{
        steps = -1;
    }
    steps++;
    position.x = shape.getPosition().x;
    position.y = shape.getPosition().y;
    if(wpn != nullptr){
        wpn->render();
    }
}

void Enemy::addWeapon(Weapon &wpn_) {
    wpn = new Gun(wpn_);
}

void Enemy::possibleAttack(DynamicObject &player) {
    if(wpn != nullptr){
        Gun *enemy_wpn = dynamic_cast<Gun *>(wpn);
        if (enemy_wpn != nullptr) {
            enemy_wpn->shoot(position);
            player.showHP(nullptr);
        }
    }
}

void Enemy::draw(sf::RenderWindow &window) {
    window.draw(shape);
    if(wpn != nullptr) {
        wpn->draw(window);
    }
}

Enemy::~Enemy() {
    counter--;
    delete wpn;
    std::cout << "Enemy destroyed " << std::endl;
}
