#ifndef SEQUENCEHANDLER_STL_GLOBAL_H
#define SEQUENCEHANDLER_STL_GLOBAL_H

#include "stdint.h"

enum class Direction{
    NONE = 0,
    FORWARD = 1,
    BARCKWARDS = 2,
    RIGHT = 3,
    LEFT = 4
};

enum class MovementMode{
    DIRECTION = 0,
    COORDINATES = 1
};

enum class Coordinate{
    X = 0,
    Y = 1,
    Z = 2

};

enum class Format{
    XML = 0,
    CSV = 1,
    ALL = 2

};


#endif // SEQUENCEHANDLER_STL_GLOBAL_H
