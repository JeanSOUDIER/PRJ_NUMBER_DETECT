#include "layer.h"


namespace BasicNN {
/*----------------------------------------------------------------------------*/
Layer::Layer(const uint64_t &cell_number){

    _cells.reserve(cell_number);

}

Layer::Layer(const std::vector<Neuron> &cells){_cells = cells;}

Layer::~Layer(){}



std::vector<Neuron> Layer::cells(){return _cells;}
uint64_t Layer::cell_number(){return _cells.size();}
void Layer::compute(){

    for(auto &neuron:_cells){

        //<PLACEHOLDER FOR CUDA CODE>
        (void)neuron; //Disables compiler warning

    }




}

void Layer::addNeuron(const Neuron& neuron){_cells.push_back(Neuron(neuron));}
/*----------------------------------------------------------------------------*/
}

