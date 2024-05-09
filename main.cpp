#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <memory>
#include <string>
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

enum PlayStates {playing,lost,won};

class Text{
private:
    sf::Font font;
    sf::Text text;
public:
    Text(){
        font.loadFromFile("resources/KGBlankSpaceSolid.ttf");
        text.setFont(font);
        text.setString("");
        text.setCharacterSize(22);
        text.setFillColor(sf::Color::White);
        text.setPosition({0,0});
    }

    Text(const std::string& str, const sf::Vector2f position) {
        font.loadFromFile("resources/KGBlankSpaceSolid.ttf");
        text.setFont(font);
        text.setString(str);
        text.setCharacterSize(22);
        text.setFillColor(sf::Color::White);
        text.setPosition(position);
    }

    Text(const Text& other){
        font.loadFromFile("resources/KGBlankSpaceSolid.ttf");
        text.setFont(font);
        text.setString(other.text.getString());
        text.setCharacterSize(22);
        text.setFillColor(sf::Color::White);
        text.setPosition(other.text.getPosition());
    }

    Text& operator= (const Text& other){
        font.loadFromFile("resources/KGBlankSpaceSolid.ttf");
        text.setFont(font);
        text.setString(other.text.getString());
        text.setCharacterSize(22);
        text.setFillColor(sf::Color::White);
        text.setPosition(other.text.getPosition());
        return *this;
    }

    sf::Vector2f getPosition() const {return text.getPosition();}
    sf::Text& getText() { return text;}

    void setSize(unsigned int size){
        text.setCharacterSize(size);
    }

    void draw(sf::RenderWindow& window){
        window.draw(text);
    }

};

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
        shape.setFillColor(sf::Color::Magenta);
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
    sf::Vector2f getSize() const { return size; }
    sf::RectangleShape& getShape() {return shape;}
    Collision& getCollisionBox(){return collisionBox;}

    virtual void draw(sf::RenderWindow& window){ window.draw(shape);}

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

    //void setDurability(unsigned int durability_) { durability = durability_; }
    //unsigned int getDurability() const { return durability; }

    ~Platform() override{
        std::cout << "Platform was destroyed " << *this;
    }
};

class DynamicObject : public Object {
protected:
    Text hp_text;
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
        shape.setFillColor(sf::Color::Cyan);
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

    void showHP(sf::RenderWindow* window) {
        if(window != nullptr) {
            hp_text.getText().setString(std::to_string(hp));
            hp_text.getText().setOrigin(this->getShape().getOrigin());
            hp_text.getText().setPosition(this->position);
            hp_text.draw(*window);
        }
    }

    unsigned int getHp() const {return hp;}
    void setHp(unsigned int hp_) {hp = hp_;}
    //float getSpeed() const {return speed;}
    //void setSpeed(float speed_) {speed = speed_;}

    bool checkHP() const{
        return hp==0;
    }

    ~DynamicObject() override{
        std::cout << "DynamicObject destroyed " << std::endl;
    }
};

class Weapon {
protected:
    int attackDamage;
    unsigned int durability;
    bool canAttack;
    std::chrono::steady_clock::time_point lastAttackTime;
    const std::chrono::steady_clock::duration attackCooldown;

    virtual void print(std::ostream& os) const {
        os << "Weapon : attackDamage = " << attackDamage << " and durability = " << durability;
    }

public:
    Weapon() : attackDamage{0}, durability{0}, canAttack{false}, attackCooldown{std::chrono::seconds(1)} {
        std::cout << "Weapon created NULL" << std::endl;
    }
    Weapon(int attackDamage_, unsigned int durability_) : attackDamage{attackDamage_}, durability{durability_},
                                                          canAttack{true}, attackCooldown{std::chrono::seconds(1)} {
        std::cout << "Weapon created " << *this;
    }
    Weapon(const Weapon& other) : attackDamage{other.attackDamage}, durability{other.durability},
                                  canAttack{other.canAttack}, attackCooldown{std::chrono::seconds(1)}{
        std::cout << "Weapon copied "<< *this;
    }

    Weapon& operator= (const Weapon& other) {
        attackDamage = other.attackDamage;
        durability = other.durability;
        canAttack = other.canAttack;
        std::cout << "Weapon operator= " << *this;
        return *this;
    }

    [[nodiscard]] virtual Weapon* clone() const { return new Weapon(*this); }

//    int getAttackDamage() const { return attackDamage; }
//    void setAttackDamage(int attackDamage_) { attackDamage = attackDamage_; }
//    unsigned int getDurability() const { return durability; }
//    void setDurability(unsigned int durability_) { durability = durability_; }

    void attack(DynamicObject& target) {
        if(canAttack) {
            canAttack = false;
            target.setHp(target.getHp() - this->attackDamage);
            std::cout<<target.getHp()<<std::endl;
            std::cout << "ATTACK" << std::endl;

            lastAttackTime = std::chrono::steady_clock::now();
            std::thread cooldownTimer([this]() {
                std::this_thread::sleep_for(attackCooldown);
                canAttack = true;
                std::cout<<"COOLDOWN ENDED\n";
            });
            cooldownTimer.detach();
        }
        else{
            std::cout << "Wait for cooldown" << std::endl;
        }
    }

    virtual void draw(sf::RenderWindow& window) {
        Object obj;
        obj.draw(window);
    }
    virtual void render() {}
    virtual void deallocate() {}

    friend std::ostream& operator<< (std::ostream& os, const Weapon& obj) {
        obj.print(os);
        os<<std::endl;
        return os;
    }

    virtual ~Weapon() {
        std::cout << "Weapon was destroyed" << std::endl;
    }
};

class Controllable{
protected:
    Weapon* wpn;
public:
    Controllable(): wpn{nullptr} {}

    //explicit Controllable(Weapon& wpn_) { wpn = new Weapon(wpn_);}

    Controllable(const Controllable& other){
        if(other.wpn != nullptr) {
            wpn = other.wpn->clone();
        }
        else{
            wpn = nullptr;
        }
        std::cout<<"Controllable copied\n";
    }

    Controllable& operator= (const Controllable& other) {
        if(this != &other) {
            std::cout << "Controllable operator=\n";
            Weapon *aux = other.getWeapon();
            if (aux != nullptr) {
                delete this->wpn;
                this->wpn = aux;
            }
        }
        return *this;
    }

    Controllable& operator= (Controllable* other){
        std::cout<<"Controllable swap\n";
        swap(*this,*other);
        delete other->getWeapon();
        return *this;
    }

    friend void swap(Controllable &c1, Controllable &c2) {
        std::swap(c1.wpn,c2.wpn);
    }

    [[nodiscard]] Weapon* getWeapon() const {return wpn;}
    virtual void addWeapon(Weapon& wpn_) = 0;

    virtual void possibleAttack(DynamicObject& obj) = 0;

    virtual void move() = 0;

    virtual ~Controllable() = default;

};

class Bullet : public DynamicObject, public Controllable{
private:
    int range;
public:
    explicit Bullet(sf::Vector2f position): DynamicObject{position,{20,10},0,5},
                                            Controllable(), range{150}{
        shape.setFillColor(sf::Color::White);
        std::cout<<"Bullet Created\n";
    }

    void move() override {
        shape.move(-speed,0);
        range--;
        if(range == 0){
            wpn->deallocate();
        }
    }

    void possibleAttack(DynamicObject& target) override {
        if(wpn != nullptr){
            if(this->getCollisionBox().checkCollision(target.getCollisionBox(),1.0)) {
                std::cout<<"YOU'VE BEEN SHOT"<<std::endl;
                wpn->attack(target);
                wpn->deallocate();
            }
        }
    }

    void addWeapon(Weapon& wpn_) override{ wpn = &wpn_; }

    ~Bullet() override {
        std::cout<<"Bullet destroyed\n";
    }

};

class Gun : public Weapon{
    Bullet* bullet;

    void print(std::ostream& os) const override {
        os << "Gun : attackDamage = " << attackDamage << " and durability = " << durability;
    }

public:
    Gun() : Weapon(), bullet{nullptr}{
        std::cout << "Gun created NULL" << std::endl;
    }
    Gun(int attackDamage_, unsigned int durability_) :  Weapon{attackDamage_, durability_}, bullet{nullptr} {
        std::cout << "Gun created " << *this;
    }
    explicit Gun(const Weapon& wpn_) :  Weapon{wpn_}, bullet{nullptr} {
        std::cout<<"Gun copied "<< *this;
    }

    [[nodiscard]] Weapon* clone() const override { return new Gun(*this); }

    [[nodiscard]] Bullet* getBullet() const{ return bullet; }

    void shoot(const sf::Vector2f position){
        if(bullet == nullptr){
            bullet = new Bullet(position);
            bullet->addWeapon(*this);
        }
    }

    void draw(sf::RenderWindow& window) override{
        if(bullet != nullptr){
            bullet->draw(window);
        }
    }
    void render() override{
        if(bullet != nullptr){
            bullet->move();
        }
    }

    void deallocate() override {
        delete bullet;
        bullet = nullptr;
    }

    ~Gun() override {
        std::cout<<"Gun destroyed\n";
    }

};

class Player : public DynamicObject, public Controllable{
public:
    Player(sf::Vector2f position_, sf::Vector2f size_, unsigned int hp_, float speed_) :
    DynamicObject{position_, size_, hp_, speed_}, Controllable() {
        std::cout << "Player created " << std::endl;
        shape.setFillColor(sf::Color::Green);
    }

    void move() override{
        float relative_speed = speed;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
            relative_speed = speed*1.5f;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)){
            shape.move(-relative_speed,0);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){
            shape.move(relative_speed,0);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)){
            shape.move(0,-relative_speed);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)){
            shape.move(0,relative_speed);
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

    void addWeapon(Weapon& wpn_) override {
        wpn = new Weapon(wpn_);
    }

    void possibleAttack(DynamicObject& target) override{
        if(wpn != nullptr){
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)){
                wpn->attack(target);
            }
        }
    }

    ~Player() override{
        delete wpn;
        std::cout << "Player destroyed " << std::endl;
    }

};

class Enemy : public DynamicObject, public Controllable{
private:
    static Text ct_text;
    static int counter;
    int steps;
    int stepsMax;
public:
    Enemy(sf::Vector2f position_, sf::Vector2f size_, unsigned int hp_, float speed_) :
    DynamicObject{position_, size_, hp_, speed_}, Controllable(), steps{0}, stepsMax{400} {
        std::cout << "Enemy created " << std::endl;
        shape.setFillColor(sf::Color::Red);
        counter++;
    }
    Enemy(const Enemy& other) : DynamicObject{other}, Controllable{other}, steps{0}, stepsMax{other.stepsMax} {
        std::cout << "Enemy copied " << std::endl;
        counter++;
    }

    Enemy& operator= (const Enemy& other) {
        DynamicObject::operator=(other);
        Controllable::operator=(other);
        std::cout << " Enemy operator= ";
        return *this;
    }

    static void updateCounter(sf::RenderWindow& window) {
        ct_text.getText().setString("Enemies remaining : " + std::to_string(counter));
        ct_text.draw(window);
    }
    static int getCounter(){ return counter;}

    void move() override{
        if(steps / (stepsMax / 4) == 0){
            shape.move(+speed,-speed/4);
            if(steps==stepsMax-1){steps=(-1)*stepsMax;}
        }
        else if(steps / (stepsMax / 4) == 1){
            shape.move(+speed,+speed/4);
        }
        else if(steps / (stepsMax / 4) == 2){
            shape.move(-speed,+speed/4);
        }
        else if(steps / (stepsMax / 4) == 3){
            shape.move(-speed,-speed/4);
        }
        else{
            steps = -1;
        }
        steps++;
        position.x = shape.getPosition().x;
        position.y = shape.getPosition().y;
        if(wpn != nullptr){
            wpn->render();
        }
    }

    void addWeapon(Weapon& wpn_) override {
        wpn = new Gun(wpn_);
    }

    void possibleAttack(DynamicObject& player) override{
        if(wpn != nullptr){
            Gun *enemy_wpn = dynamic_cast<Gun *>(wpn);
            if (enemy_wpn != nullptr) {
                enemy_wpn->shoot(position);
                player.showHP(nullptr);
            }
        }
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(shape);
        if(wpn != nullptr) {
            wpn->draw(window);
        }
    }

    ~Enemy() override{
        counter--;
        delete wpn;
        std::cout << "Enemy destroyed " << std::endl;
    }

};

int Enemy::counter = 0;
Text Enemy::ct_text{"Enemies remaining : ", {-50,-100}};

class Level{
private:
    Player& player;
    std::vector<std::shared_ptr<Text>> instructions;
    std::vector<std::shared_ptr<Object>> platforms;
    std::vector<std::shared_ptr<Object>> dynamicObjects;
    std::vector<std::shared_ptr<Enemy>> enemies;

    void init_instructions(){
        std::shared_ptr<Text> text_ptr;
        text_ptr = std::make_shared<Text>(Text{"Move with WASD", {400,-50}});
        instructions.push_back(text_ptr);
        text_ptr = std::make_shared<Text>(Text{"Press E to attack (punch) ",{350,80}});
        instructions.push_back(text_ptr);
    }

    void init_platforms(){
        std::shared_ptr<Object> ptr;
        ptr = std::make_shared<Platform>(Platform{{100,400},{200,50},100});
        platforms.push_back(ptr);
        ptr = std::make_shared<Platform>(Platform{{300,400},{200,50},100});
        platforms.push_back(ptr);
        ptr = std::make_shared<Platform>(Platform{{500,20},{200,50},100});
        platforms.push_back(ptr);
        ptr = std::make_shared<Platform>(Platform{{100,150},{200,50},100});
        platforms.push_back(ptr);
        ptr = std::make_shared<Platform>(Platform{{200,600},{200,50},100});
        platforms.push_back(ptr);

        ptr = std::make_shared<Platform>(Platform{{-500,-500},{5000,20},100});
        platforms.push_back(ptr);
        ptr = std::make_shared<Platform>(Platform{{1500,-500},{20,5000},100});
        platforms.push_back(ptr);
        ptr = std::make_shared<Platform>(Platform{{1500,1500},{5000,20},100});
        platforms.push_back(ptr);
        ptr = std::make_shared<Platform>(Platform{{-500,1500},{20,5000},100});
        platforms.push_back(ptr);
    }

    void init_objects(){
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

    void init_enemy(){
        std::shared_ptr<Enemy> enemy_ptr;
        Gun gun = Gun{100,100};

        enemy_ptr = std::make_shared<Enemy>(Enemy{{500,400},{120,120},100,1});
        enemies.push_back(enemy_ptr);
        enemy_ptr = std::make_shared<Enemy>(Enemy{{500,700},{120,120},100,1});
        enemies.push_back(enemy_ptr);

        Enemy e1 = Enemy{{800,400},{100,100},100,1};
        e1.addWeapon(gun);
        enemy_ptr = std::make_shared<Enemy>(e1);
        enemies.push_back(enemy_ptr);

        Enemy e2 = Enemy{{800,700},{100,100},100,1};
        e2.addWeapon(gun);
        enemy_ptr = std::make_shared<Enemy>(e2);
        enemies.push_back(enemy_ptr);

    }

public:
    explicit Level(Player& player_) : player{player_} {
        init_instructions();
        init_platforms();
        init_objects();
        init_enemy();
    }

    bool checkCollisionsPlayerPlatforms() {
        bool collide = false;
        for (auto &platform: platforms) {
            if (platform->getCollisionBox().checkCollision(player.getCollisionBox(), 1.0)) {collide = true;}
        }
        return collide;
    }

    bool checkCollisionsPlayer(auto& objects) {
        bool collide = false;
        for (unsigned long int i=0;i<objects.size();i++) {
            auto& obj = objects[i];
            if (obj->getCollisionBox().checkCollision(player.getCollisionBox(), 0.5)) {
                collide = true;
                std::shared_ptr<DynamicObject> d_obj = std::dynamic_pointer_cast<DynamicObject>(obj);
                if(d_obj != nullptr) {
                    player.possibleAttack(*d_obj);
                    if (d_obj->checkHP()) {
                        objects.erase(objects.begin() + i);
                    }
                }
            }
        }
        return collide;
    }

    bool checkCollisions(auto& objects) {
        bool collide = false;
        for (unsigned long long i=0;i<objects.size();i++) {
            for(unsigned long long j=i+1;j<objects.size();j++){
                if (objects[i]->getCollisionBox().checkCollision(objects[j]->getCollisionBox(), 0.5)) {collide = true;}
            }
        }
        return collide;
    }

    bool checkCollisions(auto& objects1, auto& objects2, float resistance){
        bool collide = false;
        for(auto & obj1 : objects1){
            for (auto & obj2 : objects2) {
                if (obj2->getCollisionBox().checkCollision(obj1->getCollisionBox(), resistance)) {collide = true;}
            }
        }
        return collide;
    }

    void checkAllCollisions(){
        checkCollisionsPlayerPlatforms();
        checkCollisionsPlayer(dynamicObjects);
        checkCollisionsPlayer(enemies);
        checkCollisions(enemies);
        checkCollisions(dynamicObjects);
        checkCollisions(enemies,platforms,1.0);
        checkCollisions(enemies,dynamicObjects,0.5);
        checkCollisions(dynamicObjects,platforms,1.0);
    }

    void renderEnemies(PlayStates& play_state){
        for(auto & enemy : enemies){
            if(enemy != nullptr) {
                enemy->move();
                enemy->possibleAttack(player);
                Gun* enemy_wpn = dynamic_cast<Gun*>(enemy->getWeapon());
                if(enemy_wpn != nullptr){
                    Bullet* bullet = enemy_wpn->getBullet();
                    if(bullet != nullptr){
                        bullet->possibleAttack(player);
                        if (player.checkHP()) {
                            play_state = lost;
                            return;
                        }
                        for (unsigned long int i=0;i<dynamicObjects.size();i++) {
                            auto &obj = dynamicObjects[i];
                            std::shared_ptr<DynamicObject> d_obj = std::dynamic_pointer_cast<DynamicObject>(obj);
                            if (d_obj != nullptr) {
                                bullet->possibleAttack(*d_obj);
                                if (d_obj->checkHP()) {
                                    dynamicObjects.erase(dynamicObjects.begin() + i);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    void draw(sf::RenderWindow& window){
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
    }

    ~Level() = default;

};

void gameplay(sf::RenderWindow& window,sf::View& view, Player& player, std::shared_ptr<Level>& level, PlayStates& play_state){
    level->renderEnemies(play_state);
    player.move();
    player.drag(window);
    view.setCenter(player.getPosition());

    level->checkAllCollisions();

    if (Enemy::getCounter()==0) { play_state = won; }

    window.clear(sf::Color(32, 32, 32));
    player.draw(window);
    level->draw(window);
    Enemy::updateCounter(window);
    window.setView(view);
    window.display();
}

int main() {
    ////////////////////////////////////////////////////////////////////////
    /// NOTE: this function call is needed for environment-specific fixes //
    //init_threads();                                                       //
    ////////////////////////////////////////////////////////////////////////

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

    Text text_respawn{"Press R to respawn",{310,500}};
    Text text_you_won{"You won!",{300,300}};
    Text text_you_lost{"You lost :(",{300,300}};
    Text text_final;

    Player player{{100,100},{100,100},100,5};
    Weapon wpn = Weapon{50,100};
    player.addWeapon(wpn);
    std::shared_ptr<Level> level = std::make_shared<Level>(Level{player});

    PlayStates play_state = playing;

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

        if (play_state == playing) {
            gameplay(window,view,player,level,play_state);
        }
        else {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)){
                player.setHp(100);
                player.setPosition({0,0});
                play_state = playing;
                level = std::make_shared<Level>(Level{player});
            }
            window.clear(sf::Color(32, 32, 32));
            if(play_state == won) {
                text_final = text_you_won;
            }
            else if(play_state == lost){
                text_final = text_you_lost;
            }
            text_final.setSize(50);
            text_final.draw(window);
            view.setCenter(text_final.getPosition().x+text_final.getPosition().x/2,text_final.getPosition().y+text_final.getPosition().y/2);
            text_respawn.draw(window);
            window.setView(view);
            window.display();
        }
    }
    return 0;
}
