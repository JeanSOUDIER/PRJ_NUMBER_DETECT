#include "integrator.h"


namespace Control {
/**************************************************************************/

Integrator::Integrator(System *sys, Block *input_block, const uint64_t &bind_index, const scalar timestep , const scalar initial_condition , const scalar &gain, const bool &continous) :
    Block(sys , {input_block} , std::valarray<scalar>({initial_condition}) , gain){

    _timestep = timestep;
    _previous_value = initial_condition;
    _continuous = continous;
    _bind_index = bind_index;

}

Integrator::~Integrator(){}

void Integrator::compute(){

     if(_continuous){_outputs[0] += gain() * _timestep * 0.5 * (previousValue() + input());}
     else{_outputs[0] = gain() * _timestep * 0.5 * (previousValue() + input());}

    _previous_value = _outputs[0];

}




void Integrator::reset(){

    _previous_value = 0;
    _outputs.resize(0);

}

bool Integrator::isContinuous() const{return _continuous;}
void Integrator::setContinous(const bool &continuous){_continuous = continuous;}

scalar Integrator::timestep() const{return _timestep;}
void Integrator::setTimeStep(const scalar &timestep){_timestep = timestep;}

scalar Integrator::previousValue() const{return _previous_value;}
void Integrator::setPreviousValue(const scalar &previous_value){_previous_value = previous_value;}

scalar Integrator::input() const{return inputs().at(0)->outputs()[_bind_index];}
scalar Integrator::output() const{return _outputs[0];}

void Integrator::addInput(Block *block , const uint64_t &bind_index){(void)block; (void)bind_index;} //Do nothing for this particular instanciation of Block.



/**************************************************************************/

} //namespace Control
