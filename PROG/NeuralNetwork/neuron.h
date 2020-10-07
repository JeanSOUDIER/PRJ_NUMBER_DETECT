#ifndef NEURON_H
#define NEURON_H



#include "NeuralNetwork_global.h"
#include <vector>

namespace BasicNN {
/*----------------------------------------------------------------------------*/

class Neuron{

    friend class Layer;
    friend class ConvolutionalNN;

public:

    Neuron(const BasicNN::scalar &threshold , const BasicNN::scalar &offset , const std::vector<BasicNN::scalar> &weights);
    ~Neuron();

    virtual BasicNN::scalar threshold();

    virtual BasicNN::scalar offset();
    virtual std::vector<BasicNN::scalar> weights();



protected:

    virtual void reset();

    virtual void changeThreshold(const BasicNN::scalar &value);
    virtual void changeOffset(const BasicNN::scalar &value);
    virtual void changeWeight(const unsigned &index , const BasicNN::scalar &value);

    virtual void resizeInput(const uint64_t new_size , const bool &withClear);
    virtual void loadInput(const std::vector<BasicNN::scalar> &inputs);

    BasicNN::scalar _threshold;
    BasicNN::scalar _offset;
    std::vector<BasicNN::scalar> _weights;

    BasicNN::scalar error;

    std::vector<bool> _inputs;
    bool output;

private:

};

/*----------------------------------------------------------------------------*/
}


#endif // NEURON_H
