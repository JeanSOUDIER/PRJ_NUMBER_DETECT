#include "functionblock.h"

namespace Control {
/**************************************************************************/

FunctionBlock::FunctionBlock(System *sys ,
                             const std::vector<Block*> &inputs, const std::vector<uint64_t> &bind_indexes,
                             std::function<std::valarray<scalar>(std::valarray<scalar>)> function)
    : Block(sys , inputs , 1){

    if(inputs.size() != bind_indexes.size()){throw std::invalid_argument("A bind index is needed for each input specified.");}

     _function = function;
     _bind_indexes = bind_indexes;

}


FunctionBlock::FunctionBlock(System *sys ,
                             const std::vector<Block*> &inputs  , const std::vector<uint64_t> &bind_indexes,
                             const std::valarray<scalar> &initial_conditions,
                             std::function<std::valarray<scalar>(std::valarray<scalar>)> function)

    : Block(sys , inputs , initial_conditions){

    if(inputs.size() != bind_indexes.size()){throw std::invalid_argument("A bind index is needed for each input specified.");}

     _function = function;
     _bind_indexes = bind_indexes;

}


void FunctionBlock::compute(){

    std::valarray<scalar> arg;
    arg.resize(_inputs.size());

    for(uint64_t index = 0 ; index < _inputs.size() ; index++){
        arg[index] = inputs().at(index)->outputs()[(bind_indexes().at(index))];
    }

    _outputs = _function(arg);

}


void FunctionBlock::setFunction(const std::function<std::valarray<scalar>(std::valarray<scalar>)>& function){_function = function;}
std::function<std::valarray<scalar>(std::valarray<scalar>)>& FunctionBlock::function(){return _function;}


std::vector<uint64_t> FunctionBlock::bind_indexes() const {return _bind_indexes;}

void FunctionBlock::addInput(Block *block , const uint64_t &bind_index){(void)block; (void)bind_index;} //Do nothing for this particular instanciation of Block.
/**************************************************************************/
} //namespace Control
