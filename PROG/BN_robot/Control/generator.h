#ifndef GENERATOR_H
#define GENERATOR_H

#include "block.h"

namespace Control {

class Generator : public Block {

    friend class System;

public:

    Generator(System *sys, const uint64_t &output_number = 1);
    Generator(System *sys , const std::valarray<scalar> &initial_conditions = {1});
    ~Generator() override;

    virtual void compute() override;

    void addInput(Block *block , const uint64_t &index) override;
protected:




}; //class Generator

}//namespace Control


#endif // GENERATOR_H
