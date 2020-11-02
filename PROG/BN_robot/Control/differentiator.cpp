#include "differentiator.h"


namespace Control {
/**************************************************************************/



Differentiator::Differentiator(System *sys, Block *input_block, const uint64_t &bind_index, const scalar &timestep, const scalar &initial_ouput, const scalar &initial_input, const scalar &gain, const bool &stacks)
    : Block(sys , {input_block} , std::valarray<scalar>({initial_ouput}) , gain){

    _bind_index = bind_index;

    _timestep = timestep;

    _previous_value = initial_ouput;
    _previous_input = initial_input;

    _stacks = stacks;


}
Differentiator::~Differentiator(){}

void Differentiator::compute(){

    if(stacks()){_outputs[0] += (input() - previousInput())/timestep();}
    else{_outputs[0] = (input() - previousInput())/timestep();}

    _previous_value = _outputs[0];
    _previous_input = input();

}


void Differentiator::reset(){

    _previous_value = 0;
    _outputs.resize(0);

}


bool Differentiator::stacks() const{return _stacks;}
void Differentiator::setStacks(const bool &stacks){_stacks = stacks;}

scalar Differentiator::timestep() const{return _timestep;}
void Differentiator::setTimeStep(const scalar &timestep){_timestep = timestep;}

scalar Differentiator::previousValue() const{return _previous_value;}
void Differentiator::setPreviousValue(const scalar &previous_value){_previous_value = previous_value;}


scalar Differentiator::previousInput() const{return _previous_input;}
void Differentiator::setPreviousInput(const scalar &previous_input){_previous_input = previous_input;}

scalar Differentiator::input() const{return inputs().at(0)->outputs()[_bind_index];}
scalar Differentiator::output() const{return _outputs[0];}

void Differentiator::addInput(Block *block , const uint64_t &bind_index){(void)block; (void)bind_index;} //Do nothing for this particular instanciation of Block.



/**************************************************************************/
} //namespace Control
