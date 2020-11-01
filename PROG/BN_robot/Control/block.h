#ifndef BLOCK_H
#define BLOCK_H

#include "Control_global.h"

#include "system.h"
#include <vector>
#include <valarray>

namespace Control {


class System;

class Block {

        friend class System;

public:

    Block(System *sys , const std::vector<Block*> &inputs , const uint64_t &output_number = 1, const scalar &gain = 1);
    Block(System *sys ,const std::vector<Block*> &inputs , const std::valarray<scalar> &initial_conditions = {1}, const scalar &gain = 1);
    virtual ~Block() = 0;

    void setGain(const scalar &gain);
    scalar gain() const;

    std::vector<Block*> inputs() const;

    std::valarray<scalar> outputs() const;
    scalar output(const uint64_t index) const;

    uint64_t Ts() const;

    virtual void compute() = 0;
    virtual void addInput(Block *block , const uint64_t &bind_index) =0;

    /*

void Block::addInput(Block* block, const uint64_t &index){

    _inputs.push_back(block);

    if((std::find(_sys->blocks().begin(), _sys->blocks().end(), block) != _sys->blocks().end())){
        _sys->addBlock(block);
    }

}

    */
protected:



    void resizeOutput(const uint64_t &size);


    System *_sys;
    std::vector<Block*> _inputs;
    std::valarray<scalar> _outputs;
    scalar _gain;


};//class Block

} //namespace Control



#endif // BLOCK_H
