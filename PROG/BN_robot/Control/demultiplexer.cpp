#include "demultiplexer.h"

namespace Control {
/**************************************************************************/
Demultiplexer::Demultiplexer(System *sys, Block *input, const uint64_t &bind_index, const uint64_t &output_number, const scalar &gain)
    : Block(sys , {input} , output_number , gain) {

    _input_block = input;

    _bind_index = bind_index;
    _current_output = 0;

}


Demultiplexer::Demultiplexer(System *sys , Block *input , const uint64_t &bind_index, const uint64_t &output_number, const uint64_t &initial_index, const scalar &gain)
    : Block(sys , {input} , output_number , gain) {

    _input_block = input;

    _bind_index = bind_index;
    _current_output = initial_index;

    for(auto &item:_outputs){item = 0;}
    _outputs[_current_output] = Demultiplexer::input();

}


Demultiplexer::~Demultiplexer(){}


void Demultiplexer::link(const uint64_t &index){

    if(index >= outputs().size()){throw std::invalid_argument("Tried to bind to an output outside the defined range.");}

    _current_output = index;

}

void Demultiplexer::compute(){

    for(auto &item:_outputs){item = 0;}
    _outputs[_current_output] = gain()*Demultiplexer::input();

}


scalar Demultiplexer::output() const{return _outputs[_current_output];}

scalar Demultiplexer::input() const{return _input_block->outputs()[_bind_index];}

/**************************************************************************/
} //namespace Control



