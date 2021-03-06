#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include "block.h"


namespace Control {

class Differentiator : public Block {

        friend class System;

public:

    Differentiator(System *sys ,
                   Block* input_block ,
                   const uint64_t &bind_index,
                   const scalar &timestep ,
                   const scalar &initial_ouput = 0,
                   const scalar &initial_input = 0,
                   const scalar &gain = 1,
                   const bool &stacks = false);

    ~Differentiator() override;

    void reset();

    bool stacks() const;
    void setStacks(const bool &stacks);

    scalar timestep() const;
    void setTimeStep(const scalar &timestep);

    scalar previousValue() const;
    void setPreviousValue(const scalar &previous_value);


    scalar previousInput() const;
    void setPreviousInput(const scalar &previous_input);


    scalar input() const;
    scalar output() const;

    virtual void compute() override;

    virtual void addInput(Block *block , const uint64_t &bind_index) override;

protected:




private:

    uint64_t _bind_index;

    bool _stacks;

    scalar _timestep;
    scalar _previous_value;
    scalar _previous_input;


}; //class Differentiator


} //namespace Control

#endif // DIFFERENTIATOR_H
