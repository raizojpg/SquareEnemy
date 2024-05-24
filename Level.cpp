#include "Level.h"

void Level::init_instructions() {
    std::shared_ptr<Text> text_ptr;
    text_ptr = std::make_shared<Text>(Text{"Move with WASD", {400,-50}});
    instructions.push_back(text_ptr);
    text_ptr = std::make_shared<Text>(Text{"Press E to attack (punch) ",{350,80}});
    instructions.push_back(text_ptr);
}

void Level::init_platforms() {
    std::shared_ptr<Platform> ptr;

    ptr = std::make_shared<StaticPlatform>(StaticPlatform{{100, 400}, {200, 50}, 100});
    platforms.push_back(ptr);
    ptr = std::make_shared<StaticPlatform>(StaticPlatform{{300, 400}, {200, 50}, 100});
    platforms.push_back(ptr);
    ptr = std::make_shared<StaticPlatform>(StaticPlatform{{500, 20}, {200, 50}, 100});
    platforms.push_back(ptr);
    ptr = std::make_shared<StaticPlatform>(StaticPlatform{{100, 150}, {200, 50}, 100});
    platforms.push_back(ptr);
    ptr = std::make_shared<StaticPlatform>(StaticPlatform{{200, 600}, {200, 50}, 100});
    platforms.push_back(ptr);

    ptr = std::make_shared<StaticPlatform>(StaticPlatform{{-500,-500},{5000,20},100});
    platforms.push_back(ptr);
    ptr = std::make_shared<StaticPlatform>(StaticPlatform{{1500,-500},{20,5000},100});
    platforms.push_back(ptr);
    ptr = std::make_shared<StaticPlatform>(StaticPlatform{{1500,1500},{5000,20},100});
    platforms.push_back(ptr);
    ptr = std::make_shared<StaticPlatform>(StaticPlatform{{-500,1500},{20,5000},100});
    platforms.push_back(ptr);

    ptr = std::make_shared<MovingPlatform>(MovingPlatform{{1200, 500}, {20, 700}, 100, 1, 400, true});
    platforms.push_back(ptr);
    ptr = std::make_shared<MovingPlatform>(MovingPlatform{{800, 700}, {600, 20}, 100, 1, 400, false});
    platforms.push_back(ptr);
    ptr = std::make_shared<MovingPlatform>(MovingPlatform{{1200, 1000}, {20, 700}, 100, 1, 400, true});
    platforms.push_back(ptr);
    ptr = std::make_shared<MovingPlatform>(MovingPlatform{{800, 1100}, {600, 20}, 100, 1, 400, false});
    platforms.push_back(ptr);

    ptr = std::make_shared<RotatingPlatform>(RotatingPlatform{{-200,290},{150,150},100});
    platforms.push_back(ptr);
    ptr = std::make_shared<RotatingPlatform>(RotatingPlatform{{-200,530},{150,150},100});
    platforms.push_back(ptr);

}

void Level::init_objects() {
    std::shared_ptr<Object> ptr;
    ptr = std::make_shared<DynamicObject>(DynamicObject{{100,300},{100,100},100,5});
    dynamicObjects.push_back(ptr);
    ptr = std::make_shared<DynamicObject>(DynamicObject{{100,500},{100,100},100,5});
    dynamicObjects.push_back(ptr);
    ptr = std::make_shared<DynamicObject>(DynamicObject{{400,300},{100,100},100,5});
    dynamicObjects.push_back(ptr);
    ptr = std::make_shared<DynamicObject>(DynamicObject{{300,500},{100,100},100,5});
    dynamicObjects.push_back(ptr);
}

void Level::init_enemy() {
    std::shared_ptr<Enemy> enemy_ptr;
    Sword swd = Sword{25,100};
    Gun gun = Gun{100,100};

    enemy_ptr = std::make_shared<Enemy>(Enemy{{500,400},{120,120},100,1,400});
    enemies.push_back(enemy_ptr);
    enemy_ptr = std::make_shared<Enemy>(Enemy{{500,700},{120,120},100,1,400});
    enemies.push_back(enemy_ptr);

    Enemy e1 = Enemy{{800,400},{100,100},100,1,800};
    e1.addWeapon(gun);
    enemy_ptr = std::make_shared<Enemy>(e1);
    enemies.push_back(enemy_ptr);

    Enemy e2 = Enemy{{800,700},{100,100},100,1,800};
    e2.addWeapon(gun);
    enemy_ptr = std::make_shared<Enemy>(e2);
    enemies.push_back(enemy_ptr);

    Enemy e3 = Enemy{{900,900},{100,100},100,1,800};
    e3.addWeapon(swd);
    enemy_ptr = std::make_shared<Enemy>(e3);
    enemies.push_back(enemy_ptr);

    Enemy e4 = Enemy{{500,1100},{100,100},100,1,800};
    e4.addWeapon(swd);
    enemy_ptr = std::make_shared<Enemy>(e4);
    enemies.push_back(enemy_ptr);

}

Level::Level(Player &player_) : player{player_} {
    init_instructions();
    init_platforms();
    init_objects();
    init_enemy();
}

bool Level::checkCollisionsPlayerPlatforms() {
    bool collide = false;
    for (auto &platform: platforms) {
        if (platform->getCollisionBox().checkCollision(player.getCollisionBox(), 1.0)) {collide = true;}
    }
    return collide;
}

bool Level::checkCollisionsPlayer(auto &objects) {
    bool collide = false;
    for (unsigned long int i=0;i<objects.size();i++) {
        auto& obj = objects[i];
        std::shared_ptr<DynamicObject> d_obj = std::dynamic_pointer_cast<DynamicObject>(obj);
        if(d_obj != nullptr) {
            player.possibleAttack(*d_obj);
            if (d_obj->checkHP()) {
                objects.erase(objects.begin() + i);
            }
        }
        if (obj->getCollisionBox().checkCollision(player.getCollisionBox(), 0.5)) {
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
    player.move();
    if (player.getWeapon() != nullptr) {
        player.getWeapon()->render(player);
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
                enemy->possibleAttack(player);
                if (player.checkHP()) {
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
    for(auto& instruction : instructions){
        instruction->draw(window);
    }
    player.draw(window);
    player.showHP(&window);
}


