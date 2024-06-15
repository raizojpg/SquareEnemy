#ifndef OOP_LEVELBUILDER_H
#define OOP_LEVELBUILDER_H

#include "Level.h"

class LevelBuilder {
private:
    Level level;
    static std::vector<std::shared_ptr<Text>> init_instructions();
    static std::vector<std::shared_ptr<Platform>> init_platforms(unsigned int durability);
    static std::vector<std::shared_ptr<Object>> init_objects(int hp);
    static std::vector<std::shared_ptr<Enemy>> init_enemy(int hp);

    static void checkPlayer(const Player* player);
    static void checkCollection(const auto& collection, const std::string& name);

public:
    LevelBuilder();

    LevelBuilder& add_player(Player *player);
    LevelBuilder& build_instructions();
    LevelBuilder& build_platforms(unsigned int durability);
    LevelBuilder& build_objects(int hp);
    LevelBuilder& build_enemy(int hp);
    Level* build();

};

#endif //OOP_LEVELBUILDER_H
