#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
//my classes (not all of them)
#include "Error.h"
#include "Text.h"
#include "Arms.h"
#include "Player.h"
#include "Enemy.h"
#include "LevelBuilder.h"
#include "Decorator.h"

void start_window(sf::RenderWindow& window, PlayStates& play_state){
    window.clear(sf::Color(32, 32, 32));

    Text text_start{"Press Click to START", {280, 100}};
    text_start.draw(window);

    Decorator<sf::RectangleShape,sf::CircleShape> square1{{100,100},{100,100},50};
    Decorator<sf::RectangleShape,sf::CircleShape> square2{{100,600},{100,100},50};
    Decorator<sf::RectangleShape,sf::CircleShape> square3{{700,600},{100,100},50};
    Decorator<sf::RectangleShape,sf::CircleShape> square4{{700,100},{100,100},50};
    square1.draw(window);
    square2.draw(window);
    square3.draw(window);
    square4.draw(window);
    Decorator<sf::CircleShape,sf::CircleShape> triangle{{300,300},150,50};
    triangle.draw(window);

    window.display();
    while(true){
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            play_state = playing;
            break;
        }

    }
}

void gameplay(sf::RenderWindow& window,sf::View& view, Player* player, Level* level, PlayStates& play_state){
    if(level == nullptr){
        throw GameError("\nGAME ERROR: Your game does not have a loaded level\n");
    }
    level->render(play_state);
    player->drag(window);
    view.setCenter(player->getPosition());

    level->checkAllCollisions();

    if (Enemy::getCounter()==0) { play_state = won; }

    window.clear(sf::Color(32, 32, 32));
    level->draw(window);
    Enemy::updateCounter(window);
    window.setView(view);
    window.display();
}

int main() {

    sf::RenderWindow window;
    /// NOTE: sync with env variable APP_WINDOW from .github/workflows/cmake.yml:31
    window.create(sf::VideoMode({800, 700}), "Game", sf::Style::Default);

    ///////////////////////////////////////////////////////////////////////////
    /// NOTE: mandatory use one of vsync or FPS limit (not both)            ///
    /// This is needed so we do not burn the GPU                            ///
    window.setVerticalSyncEnabled(true);                            ///
    /// window.setFramerateLimit(60);                                       ///
    ///////////////////////////////////////////////////////////////////////////

    sf::View view{sf::Vector2f(0, 0), sf::Vector2f(800, 700)};

    try {
        Text text_respawn{"Press R to respawn", {310, 500}};
        Text text_you_won{"You won!", {300, 300}};
        Text text_you_lost{"You lost :(", {300, 300}};
        Text text_final;
        Player* player = Player::getInstance({100, 100}, {100, 100}, 100, 5);
        Arms wpn = Arms{50, 100};
        player->addWeapon(wpn);

        LevelBuilder lvlBuilder;
        //std::shared_ptr<Level> level = std::make_shared<Level>(Level());
        //*level = lvlBuilder.add_player(player).build_instructions().build_platforms().build_objects().build_enemy().build();
        Level* level = lvlBuilder.add_player(player).build_instructions().build_platforms().build_objects().build_enemy().build();

        PlayStates play_state = start;

        while (window.isOpen()) {
            bool shouldExit = false;
            sf::Event e{};
            while (window.pollEvent(e)) {
                switch (e.type) {
                    case sf::Event::Closed:
                        window.close();
                        break;
                    case sf::Event::Resized:
                        std::cout << "New width: " << window.getSize().x << '\n'
                                  << "New height: " << window.getSize().y << '\n';
                        break;
                    case sf::Event::TextEntered:
                        std::cout << char(e.text.unicode);
                        if (e.key.code == sf::Keyboard::Escape)
                            shouldExit = true;
                        break;
                    default:
                        break;
                }
            }
            if (shouldExit) {
                window.close();
                break;
            }
            if(play_state == start){
                start_window(window,play_state);
            }
            else if (play_state == playing) {
                gameplay(window, view, player, level, play_state);
            }
            else {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
                    Player::removeInstance();
                    player = Player::getInstance({100, 100}, {100, 100}, 100, 5);
                    player->addWeapon(wpn);
                    level = lvlBuilder.add_player(player).build_instructions().build_platforms().build_objects().build_enemy().build();
                    play_state = playing;
                }
                window.clear(sf::Color(32, 32, 32));
                if (play_state == won) {
                    text_final = text_you_won;
                } else if (play_state == lost) {
                    text_final = text_you_lost;
                }
                text_final.setSize(50);
                text_final.draw(window);
                view.setCenter(text_final.getPosition().x + text_final.getPosition().x / 2,
                               text_final.getPosition().y + text_final.getPosition().y / 2);
                text_respawn.draw(window);
                window.setView(view);
                window.display();
            }
        }
    }catch (const ObjectError &err) {
        std::cout << err.what() << std::endl;
    }
    catch (const DynamicObjectError &err) {
        std::cout << err.what() << std::endl;
    }
    catch (const EnemyError &err) {
        std::cout << err.what() << std::endl;
    }
    catch (const WeaponError &err) {
        std::cout << err.what() << std::endl;
    }
    catch (const GameError &err) {
        std::cout << err.what() << std::endl;
    }
    catch(const std::exception& err){
        std::cout<<"\nERROR: Something went wrong :(\n"<<std::endl;
        std::cout<<err.what()<<std::endl;

    }
    Player::removeInstance();
    return 0;
}
