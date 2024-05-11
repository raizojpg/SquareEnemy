#ifndef OOP_PLATFORM_H
#define OOP_PLATFORM_H

#include "Object.h"

class Platform : public Object {
private:
    unsigned int durability;

    void print(std::ostream& os) const override;
public:
    Platform();
    Platform(sf::Vector2f position_, sf::Vector2f size_, unsigned int durability_);
    Platform(const Platform& other);
    Platform& operator= (const Platform& other);

    //void setDurability(unsigned int durability_) { durability = durability_; }
    //unsigned int getDurability() const { return durability; }

    ~Platform() override;
};

#endif //OOP_PLATFORM_H
