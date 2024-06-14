#ifndef OOP_DECORATOR_H
#define OOP_DECORATOR_H

#include "SFML/Graphics.hpp"

template<typename T, typename U>
class Decorator {
private:
    T obj;
    U dec;
public:
    Decorator(sf::Vector2f position, sf::Vector2f size, float dsize);
    Decorator(sf::Vector2f position, float size, float dsize);
    void draw(sf::RenderWindow& window);
};

template<typename T, typename U>
Decorator<T, U>::Decorator(sf::Vector2f position, sf::Vector2f size, float dsize) : obj{size}, dec{dsize}{
    obj.setOrigin(size.x/2,size.y/2);
    obj.setPosition(position);
    obj.setFillColor(sf::Color::Yellow);
    dec.setFillColor(sf::Color::Green);
    dec.setOrigin(size.x/2, size.y/2);
    dec.setPosition(position);
}

template<typename T, typename U>
Decorator<T, U>::Decorator(sf::Vector2f position, float size, float dsize) : obj{size,3}, dec{dsize}{
    obj.setOrigin(size/2,size/2);
    obj.setPosition({position.x+size,position.y});
    obj.setFillColor(sf::Color::Magenta);
    dec.setFillColor(sf::Color(160, 32, 240));
    dec.setOrigin(size/2,size/2);
    dec.setPosition({position.x+size/2 + dsize/2, position.y+size/2 + dsize/2});
    obj.rotate(90.0f);
}

template<typename T, typename U>
void Decorator<T, U>::draw(sf::RenderWindow &window) {
    window.draw(obj);
    window.draw(dec);
}

#endif //OOP_DECORATOR_H
