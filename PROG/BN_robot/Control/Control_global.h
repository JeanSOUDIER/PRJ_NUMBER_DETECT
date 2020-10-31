#ifndef CONTROL_GLOBAL_H
#define CONTROL_GLOBAL_H

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define Q_DECL_EXPORT __declspec(dllexport)
#  define Q_DECL_IMPORT __declspec(dllimport)
#else
#  define Q_DECL_EXPORT     __attribute__((visibility("default")))
#  define Q_DECL_IMPORT     __attribute__((visibility("default")))
#endif

#if defined(CONTROL_LIBRARY)
#  define CONTROL_EXPORT Q_DECL_EXPORT
#else
#  define CONTROL_EXPORT Q_DECL_IMPORT
#endif

#include <stdint.h>
#include <stdexcept>


//#define LONG_DOUBLE_PRECISION
//#define FLOATING_PRECISION

constexpr long double PI_LD = 3.141592653589793238462643383279502884L;
constexpr long double T_PI_LD = 6.2831853071795864769252867665590057683943387987502116419498891846L;

namespace Control {

    #if defined (LONG_DOUBLE_PRECISION)
      #undef FLOATING_PRECISION
      typedef long double scalar;
    #elif defined(FLOATING_PRECISION)
      typedef float scalar ;
    #else
      typedef double scalar ;
    #endif

}


#endif // CONTROL_GLOBAL_H
