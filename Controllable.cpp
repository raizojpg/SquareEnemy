#include "Controllable.h"

Controllable::Controllable() : wpn{nullptr} {}

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
        Weapon *aux = other.getWeapon();
        if (aux != nullptr) {
            delete this->wpn;
            this->wpn = aux;
        }
    }
    return *this;
}

Controllable &Controllable::operator=(Controllable *other) {
    std::cout<<"Controllable swap\n";
    swap(*this,*other);
    delete other->getWeapon();
    return *this;
}

void swap(Controllable &c1, Controllable &c2) {
    std::swap(c1.wpn,c2.wpn);
}

Weapon *Controllable::getWeapon() const {return wpn;}
