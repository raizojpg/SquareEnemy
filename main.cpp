#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <memory>
#include <SFML/Graphics.hpp>
//#include <Helper.h>
//////////////////////////////////////////////////////////////////////
/// NOTE: this include is needed for environment-specific fixes     //
/// You can remove this include and the call from main              //
/// if you have tested on all environments, and it works without it //
//#include "env_fixes.h"                                              //
//////////////////////////////////////////////////////////////////////
/// This class is used to test that the memory leak checks work as expected even when using a GUI
//class SomeClass {
//public:
//    explicit SomeClass(int) {}
//};
//SomeClass *getC() {
//    return new SomeClass{2};
//}
//////////////////////////////////////////////////////////////////////


std::ostream& operator<< (std::ostream& os, const sf::Vector2f& vec2f){
    os << "("<<vec2f.x<<":"<<vec2f.y<<") ";
    return os;
}

class Collision;
class Platform;
class Weapon;
class Object;
class DynamicObject;
class Level;

class Collision{
private:
    sf::RectangleShape* shape;
public:
    Collision() : shape{nullptr}{}
    explicit Collision(sf::RectangleShape& shape_) : shape{&shape_} {}
    Collision& operator= (const Collision& other)= default;

    template <typename Type>
    inline Type abs(Type x){return x>=0 ? x : -x;}

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

class Object{
protected:
    sf::Vector2f position;
    sf::Vector2f size;
    sf::RectangleShape shape;
    Collision collisionBox;

    virtual void print(std::ostream& os) const {
        os << "position = " << position << ", size = " << size;
    }

public:
    Object() : position{ 0,0 }, size{ 0,0 }{
        std::cout << "Object created NULL " << std::endl;
    }

    Object(sf::Vector2f position_, sf::Vector2f size_) : position{ position_ }, size{ size_ }{
        std::cout << "Object created " << *this;
        shape = sf::RectangleShape{size};
        shape.setFillColor(sf::Color::Cyan);
        shape.setOrigin(size.x/2,size.y/2);
        shape.setPosition(position);
        collisionBox = Collision{shape};
    }

    Object(const Object& other) : position{other.position}, size{other.size}, shape{other.shape}{
        collisionBox = Collision{shape};
        std::cout << "Object copied "<< *this;
    }

    Object& operator= (const Object& other) {
        position = other.position;
        size = other.size;
        shape = other.shape;
        collisionBox = Collision{shape};
        std::cout << "Object operator= "<< *this;
        return *this;
    }

    void setPosition(const sf::Vector2f position_) { position = position_; shape.setPosition(position); }
    sf::Vector2f getPosition() const { return position; }
    void setSize(const sf::Vector2f size_) { size = size_; }
    sf::Vector2f getSize() const { return size; }
    //void setShape(const sf::RectangleShape& shape_) {shape = shape_;}
    sf::RectangleShape &getShape() {return shape;}

    Collision& getCollisionBox(){return collisionBox;}

    void draw(sf::RenderWindow& window){ window.draw(shape);}

    friend std::ostream& operator<< (std::ostream& os, const Object& obj) {
        obj.print(os);
        os<<std::endl;
        return os;
    }

    virtual ~Object() {
        std::cout << "Object was destroyed " << *this;
    }

};

class Platform : public Object {
private:
    unsigned int durability;

    void print(std::ostream& os) const override {
        Object::print(os);
        os << ", durability = "<<durability;
    }
public:
    Platform() :  durability{ 0 } {
        std::cout << "Platform created NULL " << std::endl;
    }
    Platform(sf::Vector2f position_, sf::Vector2f size_, unsigned int durability_) :
            Object{position_, size_}, durability{ durability_ }{
        std::cout << "Platform created " << *this;
    }
    Platform(const Platform& other) : Object{other}, durability{other.durability}{
        std::cout << "Platform copied " << *this;
    }

    Platform& operator= (const Platform& other) {
        Object::operator=(other);
        durability = other.durability;
        std::cout << "Platform operator= "<< *this;
        return *this;
    }

    void setDurability(unsigned int durability_) { durability = durability_; }
    //unsigned int getDurability() const { return durability; }

    ~Platform() override{
        std::cout << "Platform was destroyed " << *this;
    }
};

class DynamicObject : public Object {
protected:
    unsigned int hp;
    float speed;

    void print(std::ostream& os) const override{
        Object::print(os);
        os << ", hp = " << hp << " , speed = "<< speed;
    }

public:
    DynamicObject() : hp{0}, speed{0} {
        std::cout << "DynamicObject created NULL " << std::endl;
    }
    DynamicObject(sf::Vector2f position_, sf::Vector2f size_, unsigned int hp_, float speed_) :
        Object{position_, size_}, hp{hp_}, speed{speed_} {
        std::cout << "DynamicObject created " << std::endl;
        shape.setFillColor(sf::Color::Magenta);
    }
    DynamicObject(const DynamicObject& other) : Object{other}, hp{other.hp}, speed{other.speed} {
        std::cout << "DynamicObject copied " << std::endl;
    }

    DynamicObject& operator= (const DynamicObject& other) {
        Object::operator=(other);
        hp = other.hp;
        speed = other.speed;
        std::cout << " DynamicObject operator= ";
        return *this;
    }

    unsigned int getHp() const {return hp;}
    void setHp(unsigned int hp_) {hp = hp_;}
    //float getSpeed() const {return speed;}
    void setSpeed(float speed_) {speed = speed_;}

    bool checkHP() const{
        return hp==0;
    }

    ~DynamicObject() override{
        std::cout << "DynamicObject destroyed " << std::endl;
    }
};

class Weapon {
private:
    int attackDamage;
    unsigned int durability;
    bool canAttack;
    std::chrono::steady_clock::time_point lastAttackTime;
    const std::chrono::steady_clock::duration attackCooldown;
public:
    Weapon() : attackDamage{0}, durability{0}, canAttack{true}, attackCooldown{std::chrono::seconds(1)} {
        std::cout << "Weapon created NULL" << std::endl;
    }
    Weapon(int attackDamage_, unsigned int durability_) : attackDamage{attackDamage_}, durability{durability_},
    canAttack{true}, attackCooldown{std::chrono::seconds(1)} {
        std::cout << "Weapon created ";
        print();
    }
    Weapon(const Weapon& other) : attackDamage{other.attackDamage}, durability{other.durability},
    canAttack{other.canAttack}, attackCooldown{std::chrono::seconds(1)}{
        std::cout << "Weapon copied ";
        print();
    }

    Weapon& operator= (const Weapon& other) {
        attackDamage = other.attackDamage;
        durability = other.durability;
        canAttack = other.canAttack;
        std::cout << "Weapon operator= ";
        print();
        return *this;
    }

//    int getAttackDamage() const { return attackDamage; }
//    void setAttackDamage(int attackDamage_) { attackDamage = attackDamage_; }
//    unsigned int getDurability() const { return durability; }
//    void setDurability(unsigned int durability_) { durability = durability_; }

    void attack(DynamicObject& obj) {
        if(canAttack) {
            canAttack = false;
            obj.setHp(obj.getHp() - this->attackDamage);
            std::cout<<obj.getHp()<<std::endl;
            std::cout << "ATTACK" << std::endl;

            lastAttackTime = std::chrono::steady_clock::now();
            std::thread cooldownTimer([this]() {
                std::this_thread::sleep_for(attackCooldown);
                canAttack = true;
                std::cout<<"TIME ENDED\n";
            });
            cooldownTimer.detach();
        }
        else{
            std::cout << "Wait for cooldown" << std::endl;
        }
    }

    void print() const {
        std::cout << "Weapon : attackDamage = " << attackDamage << " and durability = " << durability << std::endl;
    }
    friend std::ostream& operator<< (std::ostream& os, const Weapon& obj) {
        os<< "Weapon : attackDamage = " << obj.attackDamage << " and durability = " <<obj.durability << std::endl;
        return os;
    }

    ~Weapon() { std::cout << "Weapon was destroyed" << std::endl; }
};

/*
class Bullet : public Weapon, public DynamicObject{
public:
//    Bullet(){std::cout<<"";};
//    Bullet(int attackDamage_, unsigned int durability_) :
//        Weapon{attackDamage_,durability_}, DynamicObject{{0,0},{20,10},0,20}{}
};
*/

class Controllable{
protected:
    Weapon wpn;
public:
    Controllable(): wpn{} {}
    explicit Controllable(Weapon& wpn_) : wpn{wpn_}{}
    Controllable(const Controllable& other): wpn{other.wpn}{std::cout<<"wpn copied\n";}

    virtual void canAttack(std::shared_ptr<DynamicObject> obj) = 0;

    virtual void move() = 0;

    //const Weapon& getWpn() const {return wpn;}
    //void setWpn(const Weapon& wpn_) {wpn = wpn_;}
};

class Player : public DynamicObject, public Controllable{
public:
    Player(sf::Vector2f position_, sf::Vector2f size_, unsigned int hp_, float speed_, Weapon wpn_) :
    DynamicObject{position_, size_, hp_, speed_}, Controllable(wpn_) {
        std::cout << "Player created " << std::endl;
        shape.setFillColor(sf::Color::Magenta);
    }

    void move() override{
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

    void drag(const sf::RenderWindow& window) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            shape.setPosition(float(mousePosition.x), float(mousePosition.y));
            position.x = (float(mousePosition.x));
            position.y = (float(mousePosition.y));
        }
    }

    void canAttack(std::shared_ptr<DynamicObject> obj) override{
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)){
            wpn.attack(*obj);
        }
    }

    ~Player() override{
        std::cout << "Player destroyed " << std::endl;
    }

};

class Enemy : public DynamicObject, public Controllable{
    int steps;
    int stepsMax;
public:
    Enemy(sf::Vector2f position_, sf::Vector2f size_, unsigned int hp_, float speed_, Weapon wpn_) :
    DynamicObject{position_, size_, hp_, speed_}, Controllable(wpn_), steps{0}, stepsMax{200} {
        std::cout << "Enemy created " << std::endl;
        shape.setFillColor(sf::Color::Red);
    }
    Enemy(const Enemy& other) : DynamicObject{other}, Controllable{other}, steps{0}, stepsMax{other.stepsMax} {
        std::cout << "Enemy copied " << std::endl;
    }

    Enemy& operator= (const Enemy& other) {
        DynamicObject::operator=(other);
        std::cout << " Enemy operator= ";
        return *this;
    }

    void move() override{
        if(steps>=0){
            shape.move(+speed,0);
            steps++;
            if(steps==stepsMax-1){steps=(-1)*stepsMax;};
        }
        else{
            shape.move(-speed,0);
            steps++;
        }
    }
    void canAttack(std::shared_ptr<DynamicObject> player) override{
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)){
            wpn.attack(*player);
        }
    }

    ~Enemy() override{
        std::cout << "Enemy destroyed " << std::endl;
    }
};

class Level{
private:
    std::vector<std::shared_ptr<Object>> platforms;
    std::vector<std::shared_ptr<Object>> dynamicObjects;
    std::vector<std::shared_ptr<Object>> enemies;
public:
    Level(){
        std::shared_ptr<Object> ptr;

        ptr = std::make_shared<Platform>(Platform{{100,400},{200,50},100});
        platforms.push_back(ptr);
        ptr = std::make_shared<Platform>(Platform{{300,400},{200,50},100});
        platforms.push_back(ptr);
        ptr = std::make_shared<Platform>(Platform{{500,120},{200,50},100});
        platforms.push_back(ptr);
        ptr = std::make_shared<Platform>(Platform{{100,150},{200,50},100});
        platforms.push_back(ptr);
        ptr = std::make_shared<Platform>(Platform{{200,600},{200,50},100});
        platforms.push_back(ptr);

        ptr = std::make_shared<DynamicObject>(DynamicObject{{100,300},{100,100},100,5});
        dynamicObjects.push_back(ptr);
        ptr = std::make_shared<DynamicObject>(DynamicObject{{100,500},{100,100},100,5});
        dynamicObjects.push_back(ptr);
        ptr = std::make_shared<DynamicObject>(DynamicObject{{500,300},{100,100},100,5});
        dynamicObjects.push_back(ptr);

        ptr = std::make_shared<Enemy>(Enemy{{500,400},{100,100},100,1,{5,100}});
        enemies.push_back(ptr);
        ptr = std::make_shared<Enemy>(Enemy{{500,500},{100,100},100,1,{5,100}});
        enemies.push_back(ptr);

    }

    unsigned long int getEnemiesSize(){return enemies.size();}

    bool checkCollisionsPlayerPlatforms(Player& player) {
        bool collide = false;
        for (auto &platform: platforms) {
            if (platform->getCollisionBox().checkCollision(player.getCollisionBox(), 1.0)) {collide = true;}
        }
        return collide;
    }

    bool checkCollisionsPlayerObjects(Player& player) {
        bool collide = false;
        for (unsigned long int i=0;i<dynamicObjects.size();i++) {
            auto& object = dynamicObjects[i];
            if (object->getCollisionBox().checkCollision(player.getCollisionBox(), 0.5)) {
                collide = true;
                std::shared_ptr<DynamicObject> obj = std::dynamic_pointer_cast<DynamicObject>(object);
                player.canAttack(obj);
                if(obj->checkHP()){
                    dynamicObjects.erase(dynamicObjects.begin()+i);
                }
            }
        }
        return collide;
    }

    bool checkCollisionsPlayerEnemy(Player& player) {
        bool collide = false;
        for (unsigned long int i=0;i<enemies.size();i++) {
            auto& enemy = enemies[i];
            if (enemy->getCollisionBox().checkCollision(player.getCollisionBox(), 0.5)) {
                collide = true;
                std::shared_ptr<DynamicObject> obj = std::dynamic_pointer_cast<DynamicObject>(enemy);
                player.canAttack(obj);
                if(obj->checkHP()){
                    enemies.erase(enemies.begin()+i);
                }
            }
        }
        return collide;
    }

    bool checkCollisionsEnemyPlatforms() {
        bool collide = false;
        for(auto & enemy : enemies){
            for (auto & platform : platforms) {
                if (platform->getCollisionBox().checkCollision(enemy->getCollisionBox(), 1.0)) {collide = true;}
            }
        }
        return collide;
    }

    bool checkCollisionsEnemyObjects() {
        bool collide = false;
        for (auto& enemy : enemies) {
            for(auto& obj : dynamicObjects){
                if (enemy->getCollisionBox().checkCollision(obj->getCollisionBox(), 0.5)) {collide = true;}
            }
        }
        return collide;
    }

    bool checkCollisionsEnemyEnemy() {
        bool collide = false;
        for (unsigned long long i=0;i<enemies.size();i++) {
            for(unsigned long long j=i+1;j<enemies.size();j++){
                if (enemies[i]->getCollisionBox().checkCollision(enemies[j]->getCollisionBox(), 0.5)) {collide = true;}
            }
        }
        return collide;
    }

    //bool checkCollisionsEnemyPlayer();

    bool checkCollisionsObjectsPlatforms() {
        bool collide = false;
        for(auto & object : dynamicObjects){
            for (auto & platform : platforms) {
                if (platform->getCollisionBox().checkCollision(object->getCollisionBox(), 1.0)) {collide = true;}
            }
        }
        return collide;
    }

    bool checkCollisionsObjectsObjects() {
        bool collide = false;
        for (unsigned long long i=0;i<dynamicObjects.size();i++) {
            for(unsigned long long j=i+1;j<dynamicObjects.size();j++){
                if (dynamicObjects[i]->getCollisionBox().checkCollision(dynamicObjects[j]->getCollisionBox(), 0.5)) {collide = true;}
            }
        }
        return collide;
    }

    //bool checkCollisionsPlayerBullets(Player& p){}

    void checkAllCollisions(Player& player){
        checkCollisionsPlayerPlatforms(player);
        checkCollisionsPlayerObjects(player);
        checkCollisionsPlayerEnemy(player);
        checkCollisionsEnemyPlatforms();
        checkCollisionsEnemyObjects();
        checkCollisionsEnemyEnemy();
        checkCollisionsObjectsPlatforms();
        checkCollisionsObjectsObjects();
    }

    void moveEnemies(){
        for(auto & enemy : enemies){
            std::shared_ptr<Enemy> obj = std::dynamic_pointer_cast<Enemy>(enemy);
            obj->move();
        }
    };

    void draw(sf::RenderWindow& window){
        for(auto& platform : platforms){
            platform->draw(window);
        }
        for (auto &object: dynamicObjects) {
            object->draw(window);
        }
        for (auto &enemy: enemies) {
            enemy->draw(window);
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

    sf::Vector2f getPosition() const {return text.getPosition();}

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
    DynamicObject obj2{{100,100},{50,50},100,5};
    DynamicObject obj3{obj1};
    obj3 = obj2;
    obj1.setPosition({500,500});
    obj1.setSize({20,20});
    obj1.setHp(100);
    obj1.setSpeed(5);
    std::cout<<obj1<<obj3;
    Weapon w{10,100};

    std::cout << " --- RETURNING --- " << std::endl;
}

void gameplay(sf::RenderWindow& window,sf::View& view, Player& player, Level& level, std::vector<Text*>& instructions, bool& playing){
    level.moveEnemies();
    player.move();
    player.drag(window);
    view.setCenter(player.getPosition());

    level.checkAllCollisions(player);
    if (level.getEnemiesSize()==0) { playing = false; }

//    if(!player.getWeapon().getCanAttack()){
//        std::thread timerThread(Weapon :: attackTimer);
//    }


    window.clear();
    player.draw(window);
    level.draw(window);
    for(auto& instruction : instructions){
        instruction->draw(window);
    }
    window.setView(view);
    window.display();
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
//    Helper helper;
//    helper.help();
//    SomeClass *c = getC();
//    std::cout << c << "\n";
//    delete c;
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

    sf::View view{sf::Vector2f(0,0),sf::Vector2f(800,700)};

    Text text_instruction{"Press E to attack (the red ones)",{350,200}};
    Text text_movement{"Move with WASD", {400,50}};
    Text text_you_won{"You won !",{300,300}};
    text_you_won.setSize(50);
    std::vector<Text*> instructions;
    instructions.push_back(&text_instruction);
    instructions.push_back(&text_movement);

    Player obj{{100,100},{100,100},100,5,{25,100}};
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
            gameplay(window,view,obj,level,instructions,playing);
        }
        else {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Y)){
                playing=true;
            }
            window.clear();
            text_you_won.draw(window);
            view.setCenter(text_you_won.getPosition().x+text_you_won.getPosition().x/2,text_you_won.getPosition().y+text_you_won.getPosition().y/2);
            window.setView(view);
            window.display();
        }
    }

    return 0;
}


// object -> platform
//
//        -> dynamic + controlable -> enemy  -> boss
//                                 -> player

// weapon -> sword
//        -> bullet