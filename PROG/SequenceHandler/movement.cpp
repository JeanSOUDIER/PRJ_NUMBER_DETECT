#include "movement.h"

Movement::Movement(){

    direction = Direction::NONE;
    mode = MovementMode::DIRECTION;
    x = 0;
    y = 0;
    z = 0;
    duration_ms = 0;

}

Movement::Movement(const Movement &mov){

    direction = mov.direction;
    mode = mov.mode;
    x = mov.x;
    y = mov.y;
    z = mov.z;
    duration_ms = mov.duration_ms;

}

/*******************************************************************/

void Movement::display() const{

    std::cout << "Movement : \n";
   if(mode == MovementMode::COORDINATES){
       std::cout << "    X: " << std::to_string(x) << "\n";
       std::cout << "    Y: " << std::to_string(y) << "\n";
       std::cout << "    Z: " << std::to_string(z) << "\n";}

   else{std::cout << "    Direction: " << std::to_string(static_cast<int>(direction)) << "\n";}

   std::cout << "    Duration (ms): " << std::to_string(duration_ms) << std::endl;



}


/**************************************************************/

void Movement::setDirection(const Direction _direction){direction = _direction; }

void Movement::setCoordinates(const long int _x , const long int _y , const long int _z){x = _x; y =_y; z=_z;}
void Movement::setX(const long int _x){x = _x;}
void Movement::setY(const long int _y){y = _y;}
void Movement::setZ(const long int _z){z = _z;}

void Movement::setDuration(const uint64_t _duration_ms){duration_ms = _duration_ms;}

void Movement::setMode(const MovementMode _mode, bool withReset){

    mode = _mode;

    if(withReset){
        x = 0;
        y = 0;
        z = 0;
        direction = Direction::NONE;
    }


}

                    /*------------------------------*/

Direction Movement::getDirection(){return direction;}
Direction Movement::getDirection() const {return direction;}

long int Movement::getCoordinate(const Coordinate coor){

    switch(coor){

        case(Coordinate::X):{return x;}
        case(Coordinate::Y):{return y;}
        case(Coordinate::Z):{return z;}
        default:{return 0;}

    }

}

long int Movement::getCoordiate(const Coordinate coor) const{

    switch(coor){

        case(Coordinate::X):{return x;}
        case(Coordinate::Y):{return y;}
        case(Coordinate::Z):{return z;}
        default:{return 0;}

    }

}

long int Movement::getX(){return x;}
long int Movement::getX() const {return x;}
long int Movement::getY(){return y;}
long int Movement::getY() const {return y;}
long int Movement::getZ(){return z;}
long int Movement::getZ() const {return z;}
uint64_t Movement::getDuration(){return duration_ms;}
uint64_t Movement::getDuration() const{return duration_ms;}

MovementMode Movement::getMode(){return mode;}
MovementMode Movement::getMode() const {return mode;}

/*******************************************************************/

bool Movement::operator ==(const Movement& mov) const{return equals(*this , mov);}


bool Movement::equals (const Movement &mov1 , const Movement &mov2){return std::addressof(mov1) == std::addressof(mov2);}

bool Movement::isEquivalent(const Movement &mov) const{

    return (mode == mov.mode)&&
            (x==mov.x)&&
            (y==mov.y)&&
            (z==mov.z)&&
            (direction==mov.direction)&&
            (duration_ms == mov.duration_ms);


}

bool Movement::positionMatches(const Movement &mov) const{

    return (mode == mov.mode)&&
           (mode == MovementMode::COORDINATES) &&
           (x==mov.x)&&
           (y==mov.y)&&
           (z==mov.z);

}
