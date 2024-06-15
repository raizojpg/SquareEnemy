#ifndef OOP_PLAYER_H
#define OOP_PLAYER_H

#include "DynamicObject.h"
#include "Controllable.h"

class Player : public DynamicObject, public Controllable{
private:
    Player(sf::Vector2f position_, sf::Vector2f size_, int hp_, float speed_);
public:
    Player(const Player& player) = delete;
    Player& operator= (const Player& other) = delete;

    static Player& getInstance(sf::Vector2f position_, sf::Vector2f size_, int hp_, float speed_);

    void move() override;
    void drag(const sf::RenderWindow& window);
    void addWeapon(Weapon& wpn_) override;
    void possibleAttack(DynamicObject& target) override;
    void draw(sf::RenderWindow& window) override;
    ~Player() override;
};

#endif //OOP_PLAYER_H
