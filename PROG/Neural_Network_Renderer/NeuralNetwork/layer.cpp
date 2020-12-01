#include "layer.h"

namespace MNN {

Layer::Layer(const ste::Matrix<scalar> &weights , const ste::Matrix<scalar> &biases)
    : _weights(weights) , _biases(biases){}

Layer::~Layer(){}

//Layer::Layer(const uint64_t &cells , const uint64_t &previous_cells)
//    : _weights(cells , previous_cells) , _biases(cells , 1){}


ste::Matrix<scalar> Layer::weights(){return _weights;}
ste::Matrix<scalar> Layer::biases(){return _biases;}

uint64_t Layer::cell_number() const{return _weights.rows();}

}//namespace MNN

