#ifndef FUNCTIONBLOCK_H
#define FUNCTIONBLOCK_H

#include "block.h"

#include <functional>

namespace Control {

class FunctionBlock : public Block {

    friend class System;

public:



    FunctionBlock(System *sys ,
                  const std::vector<Block*> &inputs, const std::vector<uint64_t> &bind_indexes,
                  std::function<std::valarray<scalar>(std::valarray<scalar>)> function = std::function<std::valarray<scalar>(std::valarray<scalar>)>());

    FunctionBlock(System *sys ,
                  const std::vector<Block*> &inputs,
                  const std::vector<uint64_t> &bind_indexes,
                  const std::valarray<scalar> &initial_conditions ,
                  std::function<std::valarray<scalar>(std::valarray<scalar>)> function = std::function<std::valarray<scalar>(std::valarray<scalar>)>());

    void setFunction(const std::function<std::valarray<scalar>(std::valarray<scalar>)>& function);
    std::function<std::valarray<scalar>(std::valarray<scalar>)>& function();


    std::vector<uint64_t> bind_indexes() const;

    void compute() override;

    virtual void addInput(Block * block , const uint64_t &bind_index) override;

protected:

    std::function<std::valarray<scalar>(std::valarray<scalar>)> _function;
    std::vector<uint64_t> _bind_indexes;




private :


};//class FunctionBlock


}//namespace Control


#endif // FUNCTIONBLOCK_H
