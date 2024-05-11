#ifndef OOP_COLLISION_H
#define OOP_COLLISION_H

#include <SFML/Graphics.hpp>

class Collision{
private:
    sf::RectangleShape* shape;
public:
    Collision();
    explicit Collision(sf::RectangleShape& shape_);
    Collision& operator= (const Collision& other)= default;

    template <typename Type>
    inline Type abs(Type x);

    bool checkCollision(Collision& other, float resistence);
    sf::Vector2f getPosition();
    sf::Vector2f getHalfSize();
    void move(float dx, float dy);
};

#endif //OOP_COLLISION_H
