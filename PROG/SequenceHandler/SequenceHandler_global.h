#ifndef SEQUENCEHANDLER_GLOBAL_H
#define SEQUENCEHANDLER_GLOBAL_H

#if defined(SEQUENCEHANDLER_LIBRARY)
#  define SEQUENCEHANDLER_EXPORT Q_DECL_EXPORT
#else
#  define SEQUENCEHANDLER_EXPORT Q_DECL_IMPORT
#endif

#include "stdint.h"
#include <iostream>

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


#endif // SEQUENCEHANDLER_GLOBAL_H
