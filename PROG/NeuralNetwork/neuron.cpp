#include "neuron.h"


namespace BasicNN {
/*----------------------------------------------------------------------------*/
Neuron::Neuron(const BasicNN::scalar &threshold , const BasicNN::scalar &offset , const std::vector<BasicNN::scalar> &weights){
    _threshold = threshold;
    _offset = offset;
    _weights = weights;
    _inputs.reserve(_weights.size());

    error = 0;
    output = 0;
}

Neuron::~Neuron(){}


void Neuron::reset(){

    std::fill(_inputs.begin() , _inputs.end() , 0);
    error = 0;

}

void Neuron::changeThreshold(const BasicNN::scalar &value){_threshold = value;}
void Neuron::changeOffset(const BasicNN::scalar &value){_offset = value;}
void Neuron::changeWeight(const unsigned &index , const BasicNN::scalar &value){_weights[index] = value;}



void Neuron::resizeInput(const uint64_t new_size , const bool &withClear){


    if(withClear){
        _inputs.clear();
        _weights.clear();

        _inputs.resize(0);
        _weights.resize(0);
    }

    if(new_size > _inputs.size()){

        _inputs.reserve(new_size);
        _weights.reserve(new_size);

        for(unsigned index = 0 ; index < _inputs.size() - new_size ; index++){
            _inputs.push_back(0);
            _weights.push_back(1);
        }

    }
    else{

        _inputs.resize(new_size);
        _weights.resize(new_size);

    }


}

void Neuron::loadInput(const std::vector<BasicNN::scalar> &inputs){

    if(inputs.size() > _inputs.size()){resizeInput(inputs.size() , false);}


    _inputs.clear();


    for(BasicNN::scalar value:inputs){_inputs.push_back(value);}


}


/*----------------------------------------------------------------------------*/
}
