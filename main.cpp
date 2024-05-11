#include <iostream>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
//my classes (not all of them)
#include "Error.h"
#include "Text.h"
#include "Weapon.h"
#include "Gun.h"
#include "Player.h"
#include "Enemy.h"
#include "Level.h"

void gameplay(sf::RenderWindow& window,sf::View& view, Player& player, std::shared_ptr<Level>& level, PlayStates& play_state){
    if(level == nullptr){
        throw GameError("\nGAME ERROR: Your game does not have a loaded level\n");
    }
    level->renderEnemies(play_state);
    player.move();
    player.drag(window);
    view.setCenter(player.getPosition());

    level->checkAllCollisions();

    if (Enemy::getCounter()==0) { play_state = won; }

    window.clear(sf::Color(32, 32, 32));
    player.draw(window);
    level->draw(window);
    Enemy::updateCounter(window);
    window.setView(view);
    window.display();
}

int main() {
    try {
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
            Gun gun = Gun{100, 100};
            Enemy e = Enemy{{100, 400}, {-100, -100}, 100, -1, -100};
            e.addWeapon(gun);
        }
        catch (const ObjectError &err) {
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

        Text text_respawn{"Press R to respawn", {310, 500}};
        Text text_you_won{"You won!", {300, 300}};
        Text text_you_lost{"You lost :(", {300, 300}};
        Text text_final;
        Player player{{100, 100}, {100, 100}, 100, 5};
        Weapon wpn = Weapon{50, 100};
        player.addWeapon(wpn);

        std::shared_ptr<Level> level;
        try {
            level = std::make_shared<Level>(Level{player});
        }
        catch (const ObjectError &err) {
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

        PlayStates play_state = playing;

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

            if (play_state == playing) {
                try {
                    gameplay(window, view, player, level, play_state);
                }
                catch (const GameError &err) {
                    std::cout << err.what() << std::endl;
                    break;
                }
            } else {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
                    player.setHp(100);
                    player.setPosition({0, 0});
                    play_state = playing;
                    level = std::make_shared<Level>(Level{player});
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
    }
    catch(const std::exception& err){
        std::cout<<"\nERROR: Something went wrong :(\n"<<std::endl;
        std::cout<<err.what()<<std::endl;

    }
    return 0;
}
