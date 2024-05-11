#ifndef OOP_LEVEL_H
#define OOP_LEVEL_H

#include <vector>
#include <memory>
#include "Player.h"
#include "Enemy.h"
#include "Platform.h"

enum PlayStates {playing,lost,won};

class Level{
private:
    Player& player;
    std::vector<std::shared_ptr<Text>> instructions;
    std::vector<std::shared_ptr<Object>> platforms;
    std::vector<std::shared_ptr<Object>> dynamicObjects;
    std::vector<std::shared_ptr<Enemy>> enemies;

    void init_instructions();
    void init_platforms();
    void init_objects();
    void init_enemy();

public:
    explicit Level(Player& player_);

    bool checkCollisionsPlayerPlatforms();
    bool checkCollisionsPlayer(auto& objects);
    bool checkCollisions(auto& objects);
    bool checkCollisions(auto& objects1, auto& objects2, float resistance);
    void checkAllCollisions();

    void renderEnemies(PlayStates& play_state);
    void draw(sf::RenderWindow& window);

    ~Level() = default;
};

#endif //OOP_LEVEL_H
