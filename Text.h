#ifndef OOP_TEXT_H
#define OOP_TEXT_H

#include <SFML/Graphics.hpp>

class Text{
private:
    sf::Font font;
    sf::Text text;
public:
    Text();
    Text(const std::string& str, sf::Vector2f position);
    Text(const Text& other);
    Text& operator= (const Text& other);
    sf::Vector2f getPosition() const;
    sf::Text& getText();
    void setSize(unsigned int size);
    void draw(sf::RenderWindow& window);
};

#endif //OOP_TEXT_H
