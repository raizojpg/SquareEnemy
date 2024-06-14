#include "Text.h"

Text::Text() {
    font.loadFromFile("../resources/KGBlankSpaceSolid.ttf");
    text.setFont(font);
    text.setString("");
    text.setCharacterSize(22);
    text.setFillColor(sf::Color::White);
    text.setPosition({0,0});
}

Text::Text(const std::string &str, const sf::Vector2f position) {
    font.loadFromFile("../resources/KGBlankSpaceSolid.ttf");
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(22);
    text.setFillColor(sf::Color::White);
    text.setPosition(position);
}

Text::Text(const Text &other) {
    font.loadFromFile("../resources/KGBlankSpaceSolid.ttf");
    text.setFont(font);
    text.setString(other.text.getString());
    text.setCharacterSize(22);
    text.setFillColor(sf::Color::White);
    text.setPosition(other.text.getPosition());
}

Text &Text::operator=(const Text &other) {
    font.loadFromFile("../resources/KGBlankSpaceSolid.ttf");
    text.setFont(font);
    text.setString(other.text.getString());
    text.setCharacterSize(22);
    text.setFillColor(sf::Color::White);
    text.setPosition(other.text.getPosition());
    return *this;
}

sf::Vector2f Text::getPosition() const {return text.getPosition();}

sf::Text &Text::getText() { return text;}

void Text::setSize(unsigned int size) {
    text.setCharacterSize(size);
}

void Text::draw(sf::RenderWindow &window) {
    window.draw(text);
}
