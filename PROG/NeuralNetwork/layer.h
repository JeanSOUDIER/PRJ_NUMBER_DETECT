#ifndef LAYER_H
#define LAYER_H

#include <stdint.h>
#include <vector>

#include "neuron.h"

namespace BasicNN {
/*----------------------------------------------------------------------------*/
class Layer{

    friend class ConvolutionalNN;

public:

    explicit Layer(const uint64_t &cell_number);
    explicit Layer(const std::vector<Neuron> &cells);
    virtual ~Layer();

protected:

    std::vector<Neuron> _cells;

    virtual std::vector<Neuron> cells();
    virtual uint64_t cell_number();
    virtual void compute();

    virtual void addNeuron(const Neuron& neuron);

private:

};

/*----------------------------------------------------------------------------*/
}

#endif // LAYER_H
