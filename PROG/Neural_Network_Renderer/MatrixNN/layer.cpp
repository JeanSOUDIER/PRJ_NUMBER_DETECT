#include "layer.h"

namespace MNN {

Layer::Layer(const ste::Matrix<scalar> &weights , const ste::Matrix<scalar> &biases)
    : _weights(weights) , _biases(biases){}

Layer::~Layer(){}

const ste::Matrix<scalar>& Layer::weights() const{return _weights;}
const ste::Matrix<scalar>& Layer::biases() const{return _biases;}

ste::Matrix<scalar>& Layer::weights(){return _weights;}
ste::Matrix<scalar>& Layer::biases(){return _biases;}


void Layer::setWeights(const ste::Matrix<scalar> &newWeights){_weights = newWeights;}
void Layer::setBiases(const ste::Matrix<scalar> &newBiases){_biases = newBiases;}

uint64_t Layer::cell_number() const{return _weights.rows();}

}//namespace MNN

