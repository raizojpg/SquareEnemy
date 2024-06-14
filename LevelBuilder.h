#ifndef OOP_LEVELBUILDER_H
#define OOP_LEVELBUILDER_H

#include "Level.h"

class LevelBuilder {
private:
    Level level;
    static std::vector<std::shared_ptr<Text>> init_instructions();
    static std::vector<std::shared_ptr<Platform>> init_platforms();
    static std::vector<std::shared_ptr<Object>> init_objects();
    static std::vector<std::shared_ptr<Enemy>> init_enemy();
public:
    LevelBuilder();

    LevelBuilder& add_player(Player *player);
    LevelBuilder& build_instructions();
    LevelBuilder& build_platforms();
    LevelBuilder& build_objects();
    LevelBuilder& build_enemy();
    Level* build();

};

#endif //OOP_LEVELBUILDER_H
