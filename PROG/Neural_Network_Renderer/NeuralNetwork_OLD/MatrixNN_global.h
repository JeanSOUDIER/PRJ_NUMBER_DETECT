#ifndef MATRIXNN_GLOBAL_H
#define MATRIXNN_GLOBAL_H

#include <QtCore/qglobal.h>

#include "Matrix/Matrix.hpp"

#include <vector>

#if defined(MATRIXNN_LIBRARY)
#  define MATRIXNN_EXPORT Q_DECL_EXPORT
#else
#  define MATRIXNN_EXPORT Q_DECL_IMPORT
#endif

//#define MNN_TRAIN
//#define DOUBLE_PRECISION

namespace MNN {


    #ifdef DOUBLE_PRECISION
    typedef double scalar;
    #else
    typedef float scalar;
    #endif

#ifdef MNN_TRAIN
    //Training method
    enum class Train{

        GRADIENT_DESCENT = 0,
        SVG = GRADIENT_DESCENT,

        MOMENTUM = 1,
        MOM = MOMENTUM,

        RMS_PROP = 2,
        RMS = RMS_PROP,

        ADAM = 3,
        ADM = ADAM
    };
#endif

    ste::Matrix<scalar> sigmoid(const ste::Matrix<scalar> &weights , const ste::Matrix<scalar> &inputs , ste::Matrix<scalar> &biases);
    ste::Matrix<scalar> sigmoid(const ste::Matrix<scalar> &arg);

    ste::Matrix<scalar> sigmoid_derivative(const ste::Matrix<scalar> &arg);

    ste::Matrix<scalar> tanh(const ste::Matrix<scalar> &weights , const ste::Matrix<scalar> &inputs , ste::Matrix<scalar> &biases);
    ste::Matrix<scalar> tanh(const ste::Matrix<scalar> &arg);

    ste::Matrix<scalar> tanh_derivative(const ste::Matrix<scalar> &arg);

    ste::Matrix<scalar> elu(const ste::Matrix<scalar> &weights , const ste::Matrix<scalar> &inputs , ste::Matrix<scalar> &biases);
    ste::Matrix<scalar> elu(const ste::Matrix<scalar> &arg);

    ste::Matrix<scalar> elu_derivative(const ste::Matrix<scalar> &arg);
}

#endif // MATRIXNN_GLOBAL_H
