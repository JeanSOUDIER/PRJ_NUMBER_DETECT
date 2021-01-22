#ifndef LAYER_H
#define LAYER_H

#include "MatrixNN_global.h"

#include "Matrix/Matrix.hpp"


namespace MNN {

class Layer{

public:

    Layer(const ste::Matrix<scalar> &weights , const ste::Matrix<scalar> &biases);
    ~Layer();

    ste::Matrix<scalar> weights();
    ste::Matrix<scalar> biases();

    uint64_t cell_number() const;

protected:

    ste::Matrix<scalar> _weights;
    ste::Matrix<scalar> _biases;



}; //class Layer

}//namespace MNN


#endif // LAYER_H
