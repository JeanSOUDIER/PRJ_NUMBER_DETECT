#include "integrator.h"

namespace Control {
/**************************************************************************/

Integrator::Integrator(System *sys, Block *input_block, const uint64_t &bind_index, const scalar &timestep , const scalar &initial_output , const scalar &initial_input ,const scalar &gain, const bool &stacks) :
    Block(sys , {input_block} , std::valarray<scalar>({initial_output}) , gain){

    _bind_index = bind_index;

    _timestep = timestep;

    _previous_value = initial_output;
    _previous_input = initial_input;

    _stacks = stacks;



}

Integrator::~Integrator(){}

void Integrator::compute(){

     //if(stacks()){_outputs[0] += gain() * _timestep * 0.5 * (previousInput() + input());}
     if(stacks()){
         _outputs[0] = gain() * _timestep * input() + _previous_value;
     } else {
         _outputs[0] = gain() * _timestep * (previousInput() + input());
     }

    _previous_value = output();
    _previous_input = input();

}




void Integrator::reset(){

    _previous_value = 0;
    _previous_input = 0;
    _outputs.resize(0);

}

bool Integrator::stacks() const{return _stacks;}
void Integrator::setStacks(const bool &stacks){_stacks = stacks;}

scalar Integrator::timestep() const{return _timestep;}
void Integrator::setTimeStep(const scalar &timestep){_timestep = timestep;}

scalar Integrator::previousValue() const{return _previous_value;}
void Integrator::setPreviousValue(const scalar &previous_value){_previous_value = previous_value;}

scalar Integrator::previousInput() const{return _previous_input;}
void Integrator::setPreviousInput(const scalar &previous_input){_previous_input = previous_input;}

scalar Integrator::input() const{return inputs().at(0)->outputs()[_bind_index];}
scalar Integrator::output() const{return _outputs[0];}

void Integrator::addInput(Block *block , const uint64_t &bind_index){(void)block; (void)bind_index;} //Do nothing for this particular instanciation of Block.



/**************************************************************************/

} //namespace Control
