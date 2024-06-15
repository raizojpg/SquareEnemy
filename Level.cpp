#include "Level.h"

bool Level::checkCollisionsPlayerPlatforms() {
    bool collide = false;
    for (auto &platform: platforms) {
        if (platform->getCollisionBox().checkCollision(player->getCollisionBox(), 1.0)) {collide = true;}
    }
    return collide;
}

bool Level::checkCollisionsPlayer(auto &objects) {
    bool collide = false;
    for (unsigned long int i=0;i<objects.size();i++) {
        auto& obj = objects[i];
        std::shared_ptr<DynamicObject> d_obj = std::dynamic_pointer_cast<DynamicObject>(obj);
        if(d_obj != nullptr) {
            player->possibleAttack(*d_obj);
            if (d_obj->checkHP()) {
                objects.erase(objects.begin() + i);
            }
        }
        if (obj->getCollisionBox().checkCollision(player->getCollisionBox(), 0.5)) {
            collide = true;
        }
    }
    return collide;
}

bool Level::checkCollisions(auto &objects) {
    bool collide = false;
    for (unsigned long long i=0;i<objects.size();i++) {
        for(unsigned long long j=i+1;j<objects.size();j++){
            if (objects[i]->getCollisionBox().checkCollision(objects[j]->getCollisionBox(), 0.5)) {collide = true;}
        }
    }
    return collide;
}

bool Level::checkCollisions(auto &objects1, auto &objects2, float resistance) {
    bool collide = false;
    for(auto & obj1 : objects1){
        for (auto & obj2 : objects2) {
            if (obj2->getCollisionBox().checkCollision(obj1->getCollisionBox(), resistance)) {collide = true;}
        }
    }
    return collide;
}

void Level::checkAllCollisions() {
    checkCollisionsPlayerPlatforms();
    checkCollisionsPlayer(dynamicObjects);
    checkCollisionsPlayer(enemies);
    checkCollisions(enemies);
    checkCollisions(dynamicObjects);
    checkCollisions(enemies,platforms,1.0);
    checkCollisions(enemies,dynamicObjects,0.5);
    checkCollisions(dynamicObjects,platforms,1.0);
}

void Level::renderPlayer() {
    player->move();
    if (player->getWeapon() != nullptr) {
        player->getWeapon()->render(*player);
    }
}

void Level::renderPlatforms() {
    for(auto& platform : platforms){
        if(platform!= nullptr){
            platform->action();
        }
    }
}

void Level::renderEnemies(PlayStates &play_state) {
    for (auto &enemy: enemies) {
        if (enemy != nullptr) {
            enemy->move();
            if (enemy->getWeapon() != nullptr) {
                enemy->getWeapon()->render(*enemy);
                enemy->possibleAttack(*player);
                if (player->checkHP()) {
                    play_state = lost;
                    return;
                }
                for (unsigned long int i = 0; i < dynamicObjects.size(); i++) {
                    auto &obj = dynamicObjects[i];
                    std::shared_ptr<DynamicObject> d_obj = std::dynamic_pointer_cast<DynamicObject>(obj);
                    if (d_obj != nullptr) {
                        enemy->possibleAttack(*d_obj);
                        if (d_obj->checkHP()) {
                            dynamicObjects.erase(dynamicObjects.begin() + i);
                        }
                    }
                }
            }
        }
    }
}

void Level::render(PlayStates &play_state) {
    renderPlayer();
    renderPlatforms();
    renderEnemies(play_state);
}

void Level::draw(sf::RenderWindow &window) {
    for(auto& platform : platforms){
        platform->draw(window);
    }
    for (auto& object: dynamicObjects) {
        object->draw(window);
    }
    for (auto& enemy: enemies) {
        enemy->draw(window);
        enemy->showHP(&window);
    }
    player->draw(window);
    player->showHP(&window);
    for(auto& instruction : instructions){
        instruction->draw(window);
    }

}


void Level::setPlayer(Player *player_) {
    player = player_;
}

void Level::setInstructions(const std::vector<std::shared_ptr<Text>> &instructions_) {
    instructions = instructions_;
}

void Level::setPlatforms(const std::vector<std::shared_ptr<Platform>> &platforms_) {
    platforms = platforms_;
}

void Level::setDynamicObjects(const std::vector<std::shared_ptr<Object>> &dynamicObjects_) {
    dynamicObjects = dynamicObjects_;
}

void Level::setEnemies(const std::vector<std::shared_ptr<Enemy>> &enemies_) {
    enemies = enemies_;
}

Player *Level::getPlayer() const {
    return player;
}

const std::vector<std::shared_ptr<Platform>> &Level::getPlatforms() const {
    return platforms;
}

const std::vector<std::shared_ptr<Object>> &Level::getDynamicObjects() const {
    return dynamicObjects;
}

const std::vector<std::shared_ptr<Enemy>> &Level::getEnemies() const {
    return enemies;
}

Level::Level(Level &other) : player{other.player}, instructions{other.instructions}{
    platforms = std::move(other.platforms);
    dynamicObjects = std::move(other.dynamicObjects);
    enemies = std::move(other.enemies);
}


