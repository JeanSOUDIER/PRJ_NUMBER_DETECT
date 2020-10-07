#ifndef NEURALNETWORK_GLOBAL_H
#define NEURALNETWORK_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(NEURALNETWORK_LIBRARY)
#  define NEURALNETWORK_EXPORT Q_DECL_EXPORT
#else
#  define NEURALNETWORK_EXPORT Q_DECL_IMPORT
#endif


namespace BasicNN {
    //Should you require double floatting point precision, you can simply switch the line commented
    typedef float scalar;
    //typedef double scalar;


    enum class Propagation{
        FORWARD = 0,
        BARCKWARDS = 1
    };

}

namespace GPU_ACCELERATION {




}





#endif // NEURALNETWORK_GLOBAL_H
