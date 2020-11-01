#include "block.h"
#include "system.h"


namespace Control {
/**************************************************************************/


Block::Block(System *sys ,const std::vector<Block*> &inputs , const uint64_t &output_number, const scalar &gain){

    _sys = sys;

    _inputs = inputs;

    _gain = gain;

    _outputs.resize(output_number);

    _sys->addBlock(this);

}

Block::Block(System *sys ,const std::vector<Block*> &inputs , const std::valarray<scalar> &initial_conditions, const scalar &gain){

    _sys = sys;

    _inputs = inputs;

    _outputs = initial_conditions;

    _gain = gain;

    _sys->addBlock(this);

}

Block::~Block(){}




void Block::resizeOutput(const uint64_t &size){

    const uint64_t old_size = _outputs.size();
    const std::valarray<scalar> old_data = _outputs;

    _outputs.resize(size);

    for(uint64_t index = 0 ; index < std::min(old_size , _outputs.size()) ; index++){_outputs[index] = old_data[index];}

}





void Block::setGain(const scalar &gain){_gain = gain;}
scalar Block::gain() const{return _gain;}

std::vector<Block*> Block::inputs() const {return _inputs;}

std::valarray<scalar> Block::outputs() const{return _outputs;}
scalar Block::output(const uint64_t index) const{return _outputs[index];}
uint64_t Block::Ts() const{return _sys->Ts();}

/**************************************************************************/
} //namespace Control


