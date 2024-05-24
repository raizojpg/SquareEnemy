#include "Controllable.h"

Controllable::Controllable() : wpn{nullptr} {}

//Controllable::Controllable(Weapon &wpn_) { wpn = wpn_.clone();}

Controllable::Controllable(const Controllable &other) {
    if(other.wpn != nullptr) {
        wpn = other.wpn->clone();
    }
    else{
        wpn = nullptr;
    }
    std::cout<<"Controllable copied\n";
}

Controllable &Controllable::operator=(const Controllable &other) {
    if(this != &other) {
        std::cout << "Controllable operator=\n";
        if(other.getWeapon() != nullptr){
            Weapon *aux = other.getWeapon()->clone();
            if (aux != nullptr) {
                delete this->wpn;
                this->wpn = aux;
            }
        }
    }
    return *this;
}

Weapon *Controllable::getWeapon() const {return wpn;}

Controllable::~Controllable() {
    std::cout<<"Controllable destroyed\n";
    delete wpn;
}

