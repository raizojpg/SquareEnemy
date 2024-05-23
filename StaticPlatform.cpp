#include "StaticPlatform.h"

void StaticPlatform::action() {
    //this->shape.setPosition(position);
    if(alpha == 250 ){
        increasing = false;
    }
    if(alpha == 150) {
        increasing = true;
    }
    if(increasing){
        alpha+=step;
    }
    else{
        alpha-=step;
    }
    shape.setFillColor(sf::Color(250,0,255,alpha));
}
