#ifndef OOP_LEVEL_H
#define OOP_LEVEL_H

#include <vector>
#include <memory>
#include "Player.h"
#include "Enemy.h"
#include "StaticPlatform.h"
#include "MovingPlatform.h"
#include "RotatingPlatform.h"

enum PlayStates {start,playing,lost,won};

class Level{
private:
    Player* player;
    std::vector<std::shared_ptr<Text>> instructions;
    std::vector<std::shared_ptr<Platform>> platforms;
    std::vector<std::shared_ptr<Object>> dynamicObjects;
    std::vector<std::shared_ptr<Enemy>> enemies;

    void renderPlayer();
    void renderPlatforms();
    void renderEnemies(PlayStates& play_state);

    bool checkCollisionsPlayerPlatforms();
    bool checkCollisionsPlayer(auto& objects);
    bool checkCollisions(auto& objects);
    bool checkCollisions(auto& objects1, auto& objects2, float resistance);

    void setPlayer(Player *player);
    void setInstructions(const std::vector<std::shared_ptr<Text>> &instructions);
    void setPlatforms(const std::vector<std::shared_ptr<Platform>> &platforms);
    void setDynamicObjects(const std::vector<std::shared_ptr<Object>> &dynamicObjects);
    void setEnemies(const std::vector<std::shared_ptr<Enemy>> &enemies);

    [[nodiscard]] Player *getPlayer() const;
    [[nodiscard]] const std::vector<std::shared_ptr<Platform>> &getPlatforms() const;
    [[nodiscard]] const std::vector<std::shared_ptr<Object>> &getDynamicObjects() const;
    [[nodiscard]] const std::vector<std::shared_ptr<Enemy>> &getEnemies() const;

public:
    Level(): player{nullptr} {}
    Level(Level& other);

    void checkAllCollisions();
    void render(PlayStates& play_state);
    void draw(sf::RenderWindow& window);

    friend class LevelBuilder;

    ~Level() = default;

};

#endif //OOP_LEVEL_H
