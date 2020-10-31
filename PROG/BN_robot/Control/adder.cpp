#include "adder.h"

namespace Control {
/**************************************************************************/
Adder::Adder(System *sys, const std::vector<Block*> &inputs , const std::vector<uint64_t> &bind_indexes , const std::vector<POLARITY> &polarities , const scalar &gain)
    :Block(sys , inputs , gain){

    if(inputs.size() != bind_indexes.size()){throw std::invalid_argument("A bind index should be specified for all input.");}
    if(inputs.size() != polarities.size()){throw std::invalid_argument("A polarity should be specified for all input.");}


    _bind_indexes = bind_indexes;
    _polarities = polarities;

    _outputs[0] = 0;

}


Adder::~Adder(){}





void Adder::compute(){

    scalar result = 0;

    for(uint64_t index = 0 ; index < inputs().size() ; index++){
        result += inputs().at(index)->outputs()[bind_indexes().at(index)] * static_cast<scalar>(polarities().at(index));
    }


    _outputs[0] = gain() * result;

}

void Adder::addInput(Block * block , const uint64_t &bind_index){addInput(block , bind_index ,POLARITY::P);}
void Adder::addInput(Block *block , const uint64_t &bind_index ,  const POLARITY &polarity){


    if(bind_index >= block->outputs().size()){throw std::invalid_argument("Cannot bind to an output outside the block.");}

    if(polarity != POLARITY::PLUS && polarity != POLARITY::MINUS){throw std::invalid_argument("Invalid polarity.\n This is often due to a cast.");}

    _inputs.push_back(block);
    _bind_indexes.push_back(bind_index);
    _polarities.push_back(polarity);


}







std::vector<Adder::POLARITY> Adder::polarities() const{return _polarities;}
std::vector<uint64_t> Adder::bind_indexes() const{return _bind_indexes;}
scalar Adder::output() const{return _outputs[0];}

void Adder::setPolarity(const uint64_t &index, const POLARITY &polarity){

    if(index>=inputs().size()){throw std::invalid_argument("Cannot change the polarity of an input outside the block.");}
    if(polarity != POLARITY::PLUS && polarity != POLARITY::MINUS){throw std::invalid_argument("Invalid polarity.\n This is often due to a cast.");}

    _polarities[index] = polarity;

}

/**************************************************************************/
} //namespace Control
