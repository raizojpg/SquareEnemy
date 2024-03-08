#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>
#include <Helper.h>
//////////////////////////////////////////////////////////////////////
/// NOTE: this include is needed for environment-specific fixes     //
/// You can remove this include and the call from main              //
/// if you have tested on all environments, and it works without it //
#include "env_fixes.h"                                              //
//////////////////////////////////////////////////////////////////////
/// This class is used to test that the memory leak checks work as expected even when using a GUI
class SomeClass {
public:
    explicit SomeClass(int) {}
};
SomeClass *getC() {
    return new SomeClass{2};
}
//////////////////////////////////////////////////////////////////////

void print() { std::cout << std::endl; }

class TwoD {
private:
    unsigned int height;
    unsigned int width;
public:
    TwoD() : height{ 0 }, width{ 0 } {
        std::cout << "TwoD created NULL" << std::endl;
    }
    TwoD(unsigned int height_, unsigned int width_) : height{ height_ }, width{ width_ } {
        std::cout << "TwoD created ";
        print();
    }
    TwoD(const TwoD& other) : height{ other.height }, width{ other.width } {
        std::cout << "TwoD copied ";
        print();
    }

    TwoD& operator= (const TwoD& other) {
        height = other.height;
        width = other.width;
        std::cout << "TwoD operator= ";
        print();
        return *this;
    }

    void setHeight(unsigned int height_) { height = height_; }
    void setWidth(unsigned int width_) { width = width_; }
    unsigned int getHeight() const { return height; }
    unsigned int getWidth() const { return width; }

    void print_inl() const { std::cout << "(" << height << ":" << width << ") "; }
    void print() const { std::cout << "(" << height << ":" << width << ") " << std::endl; }
    friend std::ostream& operator<< (std::ostream& os, const TwoD& obj) {
        os << "(" << obj.height << ":" << obj.width << ") ";
        return os;
    }

    ~TwoD() {
        std::cout << "TwoD was destroyed ";
        print();
    }
};

class Platform {
private:
    TwoD position;
    TwoD size;
    unsigned int durability;
public:
    Platform() : position{ 0,0 }, size{ 0,0 }, durability{ 0 } {
        std::cout << "Platform created NULL " << std::endl;
    }
    Platform(TwoD position_, TwoD size_, unsigned int durability_) : position{ position_ }, size{ size_ }, durability{ durability_ } {
        std::cout << "Platform created ";
        print();
    }
    Platform(const Platform& other) : position{ other.position }, size{ other.size }, durability{ other.durability } {
        std::cout << "Platform copied ";
        print();
    }

    Platform& operator= (const Platform& other) {
        position = other.position;
        size = other.size;
        durability = other.durability;
        std::cout << "Platform operator= ";
        print();
        return *this;
    }

    void setPosition(const TwoD& position_) { position = position_; }
    void setSize(const TwoD& size_) { size = size_; }
    void setDurability(unsigned int durability_) { durability = durability_; }
    const TwoD getPosition() const { return position; }
    const TwoD getSize() const { return size; }
    unsigned int getDurability() const { return durability; }

    void print() const {
        std::cout << "Platform : position = ";
        position.print_inl();
        std::cout << ", size = ";
        size.print_inl();
        std::cout << ", durability = " << durability << std::endl;
    }
    friend std::ostream& operator<< (std::ostream& os, const Platform& obj) {
        os << "Platform : position = " << obj.position << ", size = " << obj.size <<
           ", durability = " << obj.durability << std::endl;
        return os;
    }

    ~Platform() {
        std::cout << "Platform was destroyed ";
        print();
    }
};

//class Level{
//private:
//    std::vector<Platform> map;
//    //std::vector<Enemy> enemies;
//    //Boss boss;
//};

class Weapon {
private:
    int attackDamage;
    unsigned int durability;
public:
    Weapon() : attackDamage{ 0 }, durability{ 0 } {
        std::cout << "Weapon created NULL" << std::endl;
    }
    Weapon(int attackDamage_, unsigned int durability) : attackDamage{ attackDamage_ }, durability{ durability } {
        std::cout << "Weapon created ";
        print();
    }
    Weapon(const Weapon& other) : attackDamage{ other.attackDamage }, durability{ other.durability } {
        std::cout << "Weapon copied ";
        print();
    }

    Weapon& operator= (const Weapon& other) {
        attackDamage = other.attackDamage;
        durability = other.durability;
        std::cout << "Weapon operator= ";
        print();
        return *this;
    }

    void setAttackDamage(int attackDamage_) { attackDamage = attackDamage_; }
    void setDurability(unsigned int durability_) { durability = durability_; }
    int getAttackDamage() const { return attackDamage; }
    unsigned int getDurability() const { return durability; }

    void print() const {
        std::cout << "Weapon : attackDamage = " << attackDamage << " and durability = " << durability << std::endl;
    }
    friend std::ostream& operator<< (std::ostream& os, const Weapon& obj) {
        os<< "Weapon : attackDamage = " << obj.attackDamage << " and durability = " <<obj.durability << std::endl;
        return os;
    }

    ~Weapon() { std::cout << "Weapon was destroyed" << std::endl; }
};

class DynamicObject {
private:
    TwoD position;
    TwoD size;
    unsigned int hp;
    unsigned int speed;
    Weapon wpn;
public:
    DynamicObject() : position{}, size{}, hp{ 0 }, speed{ 0 }, wpn{} {
        std::cout << "DynamicObject created NULL " << std::endl;
    }
    DynamicObject(TwoD position_, TwoD size_, unsigned int hp_, unsigned int speed_, Weapon wpn_) :
            position{position_}, size{size_}, hp{ 0 }, speed{ 0 }, wpn{wpn_} {
        std::cout << "DynamicObject created " << std::endl;
    }
    DynamicObject(const DynamicObject& other) :
            position{ other.position }, size{ other.size }, hp{ 0 }, speed{ 0 }, wpn{ other.wpn } {
        std::cout << "DynamicObject copied " << std::endl;
    }

    const TwoD &getPosition() const {return position;}
    void setPosition(const TwoD &position_) {position = position_;}
    const TwoD &getSize() const {return size;}
    void setSize(const TwoD &size_) {size = size_;}
    unsigned int getHp() const {return hp;}
    void setHp(unsigned int hp_) {hp = hp_;}
    unsigned int getSpeed() const {return speed;}
    void setSpeed(unsigned int speed_) {speed = speed_;}
    const Weapon &getWpn() const {return wpn;}
    void setWpn(const Weapon &wpn_) {wpn = wpn_;}


    void move() {}
    void attack() {}

    friend std::ostream& operator<< (std::ostream& os, const DynamicObject& obj) {
        os << "DynamicObject : position = " << obj.position << ", size = " << obj.size <<
           ", hp = " << obj.hp << " , speed = "<<obj.speed << std::endl;
        return os;
    }
    ~DynamicObject(){
        std::cout << "DynamicObject destroyed " << std::endl;
    }
};

void testTwoD() {
    //    TESTING TwoD class

    TwoD size1{ 1,1 };
    TwoD size2 = size1;
    TwoD size3{ 3,3 };
    size2 = size3;
    size2.print();
    size2.setHeight(2);
    size2.setWidth(2);
    size2.print();

    std::cout << " --- RETURNING --- " << std::endl;

}
void testPlatform() {
    //    TESTING Platform class

//  pentru astea trebuie sa am referinta la parametrii
//    TwoD pos{0,0};
//    TwoD size{1,1};
//    Platform p1{pos,size,100};

// pentru asta nu
    Platform p2{ TwoD{0,0},TwoD{2,2},100 };
    Platform p3{ p2 };
    p3 = p2;

    std::cout << " --- RETURNING --- " << std::endl;
}


int main() {
    ////////////////////////////////////////////////////////////////////////
    /// NOTE: this function call is needed for environment-specific fixes //
    //init_threads();                                                       //
    ////////////////////////////////////////////////////////////////////////

    //testTwoD();
    //testPlatform();

    DynamicObject d{};
    std::cout<<d;


    std::cout << " --- RETURNING --- " << std::endl;

    /*

    ///////////////////////////////////////////////////////////////////////////
    Helper helper;
    helper.help();
    SomeClass *c = getC();
    std::cout << c << "\n";
    delete c;
    ///////////////////////////////////////////////////////////////////////////

    sf::RenderWindow window;
    /// NOTE: sync with env variable APP_WINDOW from .github/workflows/cmake.yml:31
    window.create(sf::VideoMode({800, 700}), "My Window", sf::Style::Default);

    ///////////////////////////////////////////////////////////////////////////
    /// NOTE: mandatory use one of vsync or FPS limit (not both)            ///
    /// This is needed so we do not burn the GPU                            ///
    window.setVerticalSyncEnabled(true);                            ///
    /// window.setFramerateLimit(60);                                       ///
    ///////////////////////////////////////////////////////////////////////////

    while(window.isOpen()) {
        bool shouldExit = false;
        sf::Event e{};
        while(window.pollEvent(e)) {
            switch(e.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized:
                std::cout << "New width: " << window.getSize().x << '\n'
                          << "New height: " << window.getSize().y << '\n';
                break;
            case sf::Event::KeyPressed:
                std::cout << "Received key " << (e.key.code == sf::Keyboard::X ? "X" : "(other)") << "\n";
                if(e.key.code == sf::Keyboard::Escape)
                    shouldExit = true;
                break;
            default:
                break;
            }
        }
        if(shouldExit) {
            window.close();
            break;
        }
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(300ms);

        window.clear();
        window.display();
    }
    */
    return 0;
}


// object -> static -> platform
//
//        -> dynamic -> enemy  -> boss
//                   -> player

// weapon -> sword
//        -> bullet