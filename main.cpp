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
//#include "env_fixes.h"                                              //
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


std::ostream& operator<< (std::ostream& os, const sf::Vector2f& vec2f){
    os << "("<<vec2f.x<<":"<<vec2f.y<<") ";
    return os;
}

template <typename Type>
inline Type abs(Type x){return x>=0 ? x : -x;}

class Collision;
class Platform;
class Weapon;
class DynamicObject;
class Level;


class Collision{
private:
    sf::RectangleShape* shape;
public:
    Collision() : shape{nullptr}{}
    explicit Collision(sf::RectangleShape& shape_) : shape{&shape_} {}
    Collision& operator= (const Collision& other)= default;

    bool checkCollision(Collision& other, float resistence){
        sf::Vector2f thisPosition = getPosition();
        sf::Vector2f thisHalfSize = getHalfSize();
        sf::Vector2f otherPosition = other.getPosition();
        sf::Vector2f otherHalfSize = other.getHalfSize();
        float dx = abs(thisPosition.x - otherPosition.x) - (thisHalfSize.x + otherHalfSize.x);
        float dy = abs(thisPosition.y - otherPosition.y) - (thisHalfSize.y + otherHalfSize.y);
        if (dx<0 && dy<0){
            resistence = std::min(std::max(0.0f,resistence),1.0f);
            if(dx>dy){
                if(thisPosition.x - otherPosition.x < 0){
                    move(dx * (1-resistence),0);
                    other.move(-dx * resistence,0);
                }
                else{
                    move(-dx * (1-resistence),0);
                    other.move(dx * resistence,0);
                }
            }
            else{
                if(thisPosition.y - otherPosition.y < 0){
                    move(0,dy * (1-resistence));
                    other.move(0,-dy * resistence);
                }
                else {
                    move(0, -dy * (1 - resistence));
                    other.move(0, dy * resistence);
                }
            }
            return true;
        }
        return false;
    }
    sf::Vector2f getPosition(){ return (*shape).getPosition();}
    sf::Vector2f getHalfSize(){ return (*shape).getSize()/float(2);}
    void move(float dx, float dy){ shape->move(dx,dy);}
};

class Platform {
private:
    sf::Vector2f position;
    sf::Vector2f size;
    unsigned int durability;
    sf::RectangleShape shape;
    Collision collisionBox;
public:
    Platform() : position{ 0,0 }, size{ 0,0 }, durability{ 0 } {
        std::cout << "Platform created NULL " << std::endl;
    }
    Platform(sf::Vector2f position_, sf::Vector2f size_, unsigned int durability_) :
            position{ position_ }, size{ size_ }, durability{ durability_ }{
        std::cout << "Platform created ";
        print();
        shape = sf::RectangleShape{size};
        shape.setFillColor(sf::Color::Cyan);
        shape.setOrigin(size.x/2,size.y/2);
        shape.setPosition(position);
        collisionBox = Collision{shape};
    }
    Platform(const Platform& other) :
            position{other.position}, size{other.size}, durability{other.durability}, shape{other.shape}{
        collisionBox = Collision{shape};
        std::cout << "Platform copied ";
        print();
    }

    Platform& operator= (const Platform& other) {
        position = other.position;
        size = other.size;
        durability = other.durability;
        shape = other.shape;
        collisionBox = Collision{shape};
        std::cout << "Platform operator= ";
        print();
        return *this;
    }

    void setPosition(const sf::Vector2f position_) { position = position_; }
    sf::Vector2f getPosition() const { return position; }
    void setSize(const sf::Vector2f size_) { size = size_; }
    sf::Vector2f getSize() const { return size; }
    void setDurability(unsigned int durability_) { durability = durability_; }
    //unsigned int getDurability() const { return durability; }
    //void setShape(const sf::RectangleShape& shape_) {shape = shape_;}
    //const sf::RectangleShape &getShape() const {return shape;}

    Collision& getCollisionBox(){return collisionBox;}

    void draw(sf::RenderWindow& window){ window.draw(shape);}

    void print() const {
        std::cout << "Platform : position = "<<position<<", size = "<<size<< ", durability = "<<durability << std::endl;
    }
    friend std::ostream& operator<< (std::ostream& os, const Platform& obj) {
        os << "Platform : position = " << obj.position << ", size = " << obj.size
           <<", durability = " << obj.durability << std::endl;
        return os;
    }

    ~Platform() {
        std::cout << "Platform was destroyed ";
        print();
    }
};

class Weapon {
private:
    int attackDamage;
    unsigned int durability;
public:
    Weapon() : attackDamage{0}, durability{0} {
        std::cout << "Weapon created NULL" << std::endl;
    }
    Weapon(int attackDamage_, unsigned int durability_) : attackDamage{attackDamage_}, durability{durability_} {
        std::cout << "Weapon created ";
        print();
    }
    Weapon(const Weapon& other) : attackDamage{other.attackDamage}, durability{other.durability} {
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

//    int getAttackDamage() const { return attackDamage; }
//    void setAttackDamage(int attackDamage_) { attackDamage = attackDamage_; }
//    unsigned int getDurability() const { return durability; }
//    void setDurability(unsigned int durability_) { durability = durability_; }

//    void attack(Platform& obj) const{
//        obj.setDurability(obj.getDurability()-attackDamage);
//    }

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
    sf::Vector2f position;
    sf::Vector2f size;
    unsigned int hp;
    float speed;
    Weapon wpn;
    sf::RectangleShape shape;
    Collision collisionBox;
public:
    DynamicObject() : position{0,0}, size{0,0}, hp{0}, speed{0}, wpn{} {
        std::cout << "DynamicObject created NULL " << std::endl;
    }
    DynamicObject(sf::Vector2f position_, sf::Vector2f size_, unsigned int hp_, float speed_, Weapon& wpn_) :
    position{position_}, size{size_}, hp{hp_}, speed{speed_}, wpn{wpn_} {
        std::cout << "DynamicObject created " << std::endl;
        shape = sf::RectangleShape{size};
        shape.setFillColor(sf::Color::Magenta);
        shape.setOrigin(size.x/2,size.y/2);
        shape.setPosition(position);
        collisionBox = Collision{shape};
    }
    DynamicObject(const DynamicObject& other) :
    position{other.position}, size{other.size}, hp{other.hp}, speed{other.speed}, wpn{other.wpn}, shape{other.shape} {
        collisionBox = Collision{shape};
        std::cout << "DynamicObject copied " << std::endl;
    }

    DynamicObject& operator= (const DynamicObject& other) {
        position = other.position;
        size = other.size;
        hp = other.hp;
        speed = other.speed;
        wpn = other.wpn;
        shape = other.shape;
        collisionBox = Collision{shape};
        std::cout << " DynamicObject operator= ";
        return *this;
    }

    sf::Vector2f getPosition() const { return position; }
    void setPosition(const sf::Vector2f position_) { position = position_; }
    sf::Vector2f getSize() const { return size; }
    void setSize(const sf::Vector2f size_) { size = size_; }
    //unsigned int getHp() const {return hp;}
    void setHp(unsigned int hp_) {hp = hp_;}
    //float getSpeed() const {return speed;}
    void setSpeed(float speed_) {speed = speed_;}
    //const Weapon& getWpn() const {return wpn;}
    void setWpn(const Weapon& wpn_) {wpn = wpn_;}
    sf::RectangleShape& getShape() {return shape;}
    //void setShape(const sf::RectangleShape &shape_) {shape = shape_;}

    Collision& getCollisionBox(){return collisionBox;}

    void move() {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)){
            shape.move(-speed,0);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){
            shape.move(speed,0);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)){
            shape.move(0,-speed);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)){
            shape.move(0,speed);
        }
        position.x = shape.getPosition().x;
        position.y = shape.getPosition().y;
    }

    void drag(const sf::RenderWindow& window){
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            shape.setPosition(float(mousePosition.x),float(mousePosition.y));
            position.x = (float(mousePosition.x));
            position.y = (float(mousePosition.y));
        }
    }

    void draw(sf::RenderWindow& window){ window.draw(shape);}

//    void attack(Platform& platform) {
//        wpn.attack(platform);
//    }

    friend std::ostream& operator<< (std::ostream& os, const DynamicObject& obj) {
        os << "DynamicObject : position = " << obj.position << ", size = " << obj.size <<
           ", hp = " << obj.hp << " , speed = "<<obj.speed << std::endl;
        return os;
    }
    ~DynamicObject(){
        std::cout << "DynamicObject destroyed " << std::endl;
    }
};

class Level{
private:
    std::vector<Platform> platforms;
    std::vector<DynamicObject> objects;
    //std::vector<Enemy> enemies;
public:
    Level(){
        Platform platform1{{100,400},{200,50},100};
        Platform platform2{{300,400},{200,50},100};
        Platform platform3{{500,120},{200,50},100};
        Platform platform4{{100,150},{200,50},100};
        Platform platform5{{200,600},{200,50},100};
        platforms.push_back(platform1);
        platforms.push_back(platform2);
        platforms.push_back(platform3);
        platforms.push_back(platform4);
        platforms.push_back(platform5);
        Weapon none {};
        DynamicObject obj1{{100,300},{100,100},100,5,none};
        DynamicObject obj2{{100,500},{100,100},100,5,none};
        objects.push_back(obj1);
        objects.push_back(obj2);
    }

    bool checkCollisionsPlayerPlatforms(DynamicObject& obj) {
        bool collide = false;
        for (auto &platform: platforms) {
            if (platform.getCollisionBox().checkCollision(obj.getCollisionBox(), 1.0)) {collide = true;}
        }
        return collide;
    }

    bool checkCollisionsPlayerObjects(DynamicObject& obj) {
        bool collide = false;
        for (auto &object: objects) {
            if (object.getCollisionBox().checkCollision(obj.getCollisionBox(), 0.5)) {collide = true;}
        }
        return collide;
    }
    bool checkCollisionsObjectsObjects() {
        bool collide = false;
        for (unsigned long long i=0;i<objects.size();i++) {
            for(unsigned long long j=i+1;j<objects.size();j++){
                if (objects[i].getCollisionBox().checkCollision(objects[j].getCollisionBox(), 0.5)) {collide = true;}
            }
        }
        return collide;
    }

    bool checkCollisionsPlatformsObjects() {
        bool collide = false;
        for (auto & platform : platforms) {
            for(auto & object : objects){
                if (platform.getCollisionBox().checkCollision(object.getCollisionBox(), 1.0)) {collide = true;}
            }
        }
        return collide;
    }

    void checkAllCollisions(DynamicObject& obj){
        checkCollisionsPlayerPlatforms(obj);
        checkCollisionsPlayerObjects(obj);
        checkCollisionsObjectsObjects();
        checkCollisionsPlatformsObjects();
    }

    void draw(sf::RenderWindow& window){
        for(auto& platform : platforms){
            platform.draw(window);
        }
        for (auto &object: objects) {
            object.draw(window);
        }
    }

};

class Text{
private:
    sf::Font font;
    sf::Text text;
public:
    Text(const std::string& str, const sf::Vector2f position){
        font.loadFromFile("resources/KGBlankSpaceSolid.ttf");
        text.setFont(font);
        text.setString(str);
        text.setCharacterSize(22);
        text.setFillColor(sf::Color::White);
        text.setPosition(position);
    }

    void setSize(unsigned int size){
        text.setCharacterSize(size);
    }

    void draw(sf::RenderWindow& window){
        window.draw(text);
    }

};


void testPlatform() {
    //    TESTING Platform class
    Platform platform1{};
    Platform platform2{{100,400},{50,200},100};
    Platform platform3{platform1};
    platform3 = platform2;
    platform1.setPosition({500,500});
    platform1.setSize({20,20});
    platform1.setDurability(100);
    std::cout<<platform1<<platform3;

    std::cout << " --- RETURNING --- " << std::endl;
}

void testDynamicObject() {
    //    TESTING DynamicObject class
    Weapon none{};
    DynamicObject obj1{};
    DynamicObject obj2{{100,100},{50,50},100,5,none};
    DynamicObject obj3{obj1};
    obj3 = obj2;
    obj1.setPosition({500,500});
    obj1.setSize({20,20});
    obj1.setHp(100);
    obj1.setSpeed(5);
    obj1.setWpn(none);
    std::cout<<obj1<<obj3;

    std::cout << " --- RETURNING --- " << std::endl;
}


int main() {
    ////////////////////////////////////////////////////////////////////////
    /// NOTE: this function call is needed for environment-specific fixes //
    //init_threads();                                                       //
    ////////////////////////////////////////////////////////////////////////

    testPlatform();
    testDynamicObject();

    std::cout << " --- RETURNING --- " << std::endl;



    ///////////////////////////////////////////////////////////////////////////
    Helper helper;
    helper.help();
    SomeClass *c = getC();
    std::cout << c << "\n";
    delete c;
    ///////////////////////////////////////////////////////////////////////////


    sf::RenderWindow window;
    /// NOTE: sync with env variable APP_WINDOW from .github/workflows/cmake.yml:31
    window.create(sf::VideoMode({800, 700}), "Game", sf::Style::Default);

    ///////////////////////////////////////////////////////////////////////////
    /// NOTE: mandatory use one of vsync or FPS limit (not both)            ///
    /// This is needed so we do not burn the GPU                            ///
    window.setVerticalSyncEnabled(true);                            ///
    /// window.setFramerateLimit(60);                                       ///
    ///////////////////////////////////////////////////////////////////////////
/*
    sf::Font font;
    font.loadFromFile("resources/KGBlankSpaceSolid.ttf");

    sf::Text text_instruction;
    text_instruction.setFont(font);
    text_instruction.setString("Make the purple boxes collide");
    text_instruction.setCharacterSize(22);
    text_instruction.setFillColor(sf::Color::White);
    text_instruction.setPosition(400,250);
*/

    Text text_instruction{"Make the purple boxes collide",{350,200}};
    Text text_movement{"Move with WASD", {400,50}};
    Text text_you_won{"You won !",{300,300}};
    text_you_won.setSize(50);

    Weapon wpn {25,100};
    DynamicObject obj{{100,100},{100,100},100,5,wpn};
    obj.getShape().setFillColor(sf::Color::Green);
    Level level{};

    bool playing = true;
    while(window.isOpen()) {
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

        if (playing) {
            obj.move();
            obj.drag(window);

            level.checkAllCollisions(obj);
            if (level.checkCollisionsObjectsObjects()) { playing = false; }

            // using namespace std::chrono_literals;
            // std::this_thread::sleep_for(300ms);

            window.clear();
            obj.draw(window);
            level.draw(window);
            text_instruction.draw(window);
            text_movement.draw(window);
            window.display();
        }
        else {
            window.clear();
            text_you_won.draw(window);
            window.display();
        }
    }

    return 0;
}


// object -> static -> platform
//
//        -> dynamic -> enemy  -> boss
//                   -> player

// weapon -> sword
//        -> bullet