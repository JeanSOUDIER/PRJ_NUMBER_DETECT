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

///Enables training functions
//#define MNN_TRAIN

 ///If you uncomment this line, dataset handlers will read the file each time you need to get data instead of all reading at once. Do this if your dataset is larger than you can store in your RAM.
//s#define MNN_LOW_RAM

///This macro makes the program use 'double' type rather than 'float'. It is discouraged as of 2020 as GPU work with 'float'.
//#define MNN_DOUBLE_PRECISION

namespace MNN {


    #ifdef MNN_DOUBLE_PRECISION
    typedef double scalar;
    #else
    typedef float scalar;
    #endif

#ifdef MNN_TRAIN
    //Training method
    enum class Train{

        GRADIENT_DESCENT = 0,
        SGD = GRADIENT_DESCENT,

        MOMENTUM = 1,
        MOM = MOMENTUM,

        RMS_PROP = 2,
        RMS = RMS_PROP,

        ADAM = 3,
        ADM = ADAM
    };
#endif

    ///MNN::sigmoid | Returns 'sigmoid((weights*inputs)+biases)'.
    ste::Matrix<scalar> sigmoid(const ste::Matrix<scalar> &weights , const ste::Matrix<scalar> &inputs , ste::Matrix<scalar> &biases);
    ///MNN::sigmoid | Copies 'arg' and applies the sigmoid function on all its values.
    ste::Matrix<scalar> sigmoid(const ste::Matrix<scalar> &arg);

    ///sigmoid_r | Applies the sigmoid function on all the elements of 'arg' (which is thus modified).
    ste::Matrix<scalar>& sigmoid_r(ste::Matrix<scalar> &arg);

    ///sigmoid_derivative | Copies 'arg' and applies the derivative of the sigmoid function in all its values.
    ste::Matrix<scalar> sigmoid_derivative(const ste::Matrix<scalar> &arg);

    ///sigmoid_derivative_r | Applies the sigmoid function derivative on all the elements of 'arg' (which is thus modified).
    ste::Matrix<scalar>& sigmoid_derivative_r(ste::Matrix<scalar> &arg);


    ste::Matrix<scalar> tanh(const ste::Matrix<scalar> &weights , const ste::Matrix<scalar> &inputs , ste::Matrix<scalar> &biases);
    ste::Matrix<scalar> tanh(const ste::Matrix<scalar> &arg);
    ste::Matrix<scalar>& tanh_r(ste::Matrix<scalar> &arg);

    ste::Matrix<scalar> tanh_derivative(const ste::Matrix<scalar> &arg);
    ste::Matrix<scalar>& tanh_derivative_r(ste::Matrix<scalar> &arg);


    ste::Matrix<scalar> elu(const ste::Matrix<scalar> &weights , const ste::Matrix<scalar> &inputs , ste::Matrix<scalar> &biases , const scalar &parameter = 0.2);
    ste::Matrix<scalar> elu(const ste::Matrix<scalar> &arg, const scalar &parameter = 0.2);
    ste::Matrix<scalar>& elu_r(ste::Matrix<scalar> &arg , const scalar &parameter = 0.2);

    ste::Matrix<scalar> elu_derivative(const ste::Matrix<scalar> &arg, const scalar &parameter = 0.2);
    ste::Matrix<scalar>& elu_derivative_r(ste::Matrix<scalar> &arg , const scalar &parameter = 0.2);


    ste::Matrix<scalar> relu(const ste::Matrix<scalar> &weights , const ste::Matrix<scalar> &inputs , ste::Matrix<scalar> &biases);
    ste::Matrix<scalar> relu(const ste::Matrix<scalar> &arg);
    ste::Matrix<scalar>& relu_r(ste::Matrix<scalar> &arg);

    ste::Matrix<scalar> relu_derivative(const ste::Matrix<scalar> &arg);
    ste::Matrix<scalar>& relu_derivative_r(ste::Matrix<scalar> &arg);

}

#endif // MATRIXNN_GLOBAL_H
