#include "multiplexer.h"


namespace Control {
/**************************************************************************/

Multiplexer::Multiplexer(System *sys, const std::vector<Block*> &inputs , const std::vector<uint64_t> &bind_indexes , const uint64_t &initial_out_index, const scalar &gain)
    : Block(sys , inputs , 1 , gain){


    if(inputs.size() != bind_indexes.size()){throw std::invalid_argument("A bind index is needed for each input specified.");}

    _bind_indexes = bind_indexes;
    _out_selector = initial_out_index;
    _outputs[0] = active_input();

}

Multiplexer::~Multiplexer(){}

void Multiplexer::compute(){
    _outputs[0] = gain() * active_input();
}

void Multiplexer::link(const uint64_t &index){

    if(index >= inputs().size()){throw std::invalid_argument("Selected input is outside range.");}

    _out_selector = index;

}

scalar Multiplexer::active_input() const{return _inputs.at(_out_selector)->outputs()[_bind_indexes.at(_out_selector)];}

scalar Multiplexer::output() const{return _outputs[0];}

/**************************************************************************/
} //namespace Control
