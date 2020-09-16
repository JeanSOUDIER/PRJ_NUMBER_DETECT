#include "movement.h"

Movement::Movement(){

    direction = Direction::NONE;
    mode = MovementMode::DIRECTION;
    x = 0;
    y = 0;
    z = 0;
    duration_ms = 0;
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

long int Movement::getCoordinate(const Coordinate coor){

    switch(coor){

        case(Coordinate::X):{return x;}
        case(Coordinate::Y):{return y;}
        case(Coordinate::Z):{return z;}
        default:{return 0;}

    }

}

long int Movement::getX(){return x;}
long int Movement::getY(){return y;}
long int Movement::getZ(){return z;}

uint64_t Movement::getDuration(){return duration_ms;}

MovementMode Movement::getMode(){return mode;}


/*******************************************************************/

bool Movement::operator ==(const Movement& mov) const{return equals(*this , mov);}


bool Movement::equals (const Movement &mov1 , const Movement &mov2){return std::addressof(mov1) == std::addressof(mov2);}

bool Movement::equals(const Movement &mov) const{

    return (mode == mov.mode)&&
            (x==mov.x)&&
            (y==mov.y)&&
            (z==mov.z)&&
            (direction==mov.direction)&&
            (duration_ms == mov.duration_ms);


}
