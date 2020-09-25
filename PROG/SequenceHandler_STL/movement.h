#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <memory> //std::addressof

#include "SequenceHandler_STL_global.h"

class Movement{

public:

    Movement();
    Movement(const Movement&);

                    /*------------------------------*/

    void display() const;

                    /*------------------------------*/


    void setDirection(const Direction _direction);

    void setCoordinates(const long int _x , const long int _y , const long int _z);
    void setX(const long int _x);
    void setY(const long int _y);
    void setZ(const long int _z);

    void setDuration(const uint64_t _duration_ms);

    void setMode(const MovementMode _mode, bool withReset);


                    /*------------------------------*/

    Direction getDirection();
    Direction getDirection() const;

    long int getCoordinate(const Coordinate coor);
    long int getCoordiate(const Coordinate coor) const;
    long int getX();
    long int getX() const;
    long int getY();
    long int getY() const;
    long int getZ();
    long int getZ() const;

    uint64_t getDuration();
    uint64_t getDuration() const;

    MovementMode getMode();
    MovementMode getMode() const;

                    /*------------------------------*/

    bool operator ==(const Movement& mov) const;
    static bool equals (const Movement &mov1 , const Movement &mov2);
    bool isEquivalent(const Movement &mov) const;
    bool positionMatches(const Movement &mov) const;


private :

    MovementMode mode;

    Direction direction;
    long int x;
    long int y;
    long int z;
    uint64_t duration_ms;



};

#endif // MOVEMENT_H
