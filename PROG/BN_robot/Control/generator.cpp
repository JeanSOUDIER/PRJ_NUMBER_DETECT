#include "generator.h"


namespace Control {
/**************************************************************************/
Generator::Generator(System *sys, const uint64_t &output_number) :
    Block(sys , {} , output_number , 1) {

}

Generator::Generator(System *sys , const std::valarray<scalar> &initial_conditions )
    :Block(sys , {} , initial_conditions , 1){}


Generator::~Generator(){}


void Generator::compute(){

    for(auto &output:_outputs){
        output = !output;
    }

}

void Generator::addInput(Block *block , const uint64_t &index){(void)block; (void)index;} //Do nothing for this particular subclass of Block.


/**************************************************************************/
} //namespace Control

