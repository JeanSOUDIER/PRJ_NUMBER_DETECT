#include "differentiator.h"


namespace Control {
/**************************************************************************/



Differentiator::Differentiator(System *sys, Block *input_block, const uint64_t &bind_index, const scalar timestep, const scalar initial_condition, const scalar &gain, const bool &continous)
    : Block(sys , {input_block} , std::valarray<scalar>({initial_condition}) , gain){

    _timestep = timestep;
    _previous_value = initial_condition;
    _continuous = continous;
    _bind_index = bind_index;

}
Differentiator::~Differentiator(){}

void Differentiator::compute(){

    _outputs[0] = (input() - previousValue())/timestep();

    _previous_value = _outputs[0];

}


void Differentiator::reset(){

    _previous_value = 0;
    _outputs.resize(0);

}


bool Differentiator::isContinuous() const{return _continuous;}
void Differentiator::setContinous(const bool &continuous){_continuous = continuous;}

scalar Differentiator::timestep() const{return _timestep;}
void Differentiator::setTimeStep(const scalar &timestep){_timestep = timestep;}

scalar Differentiator::previousValue() const{return _previous_value;}
void Differentiator::setPreviousValue(const scalar &previous_value){_previous_value = previous_value;}


scalar Differentiator::input() const{return inputs().at(0)->outputs()[_bind_index];}
scalar Differentiator::output() const{return _outputs[0];}

void Differentiator::addInput(Block *block , const uint64_t &bind_index){(void)block; (void)bind_index;} //Do nothing for this particular instanciation of Block.



/**************************************************************************/
} //namespace Control
