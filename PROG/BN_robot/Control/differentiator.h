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
                   const scalar timestep ,
                   const scalar initial_condition ,
                   const scalar &gain,
                   const bool &continous);

    ~Differentiator() override;

    void reset();

    bool isContinuous() const;
    void setContinous(const bool &continuous);

    scalar timestep() const;
    void setTimeStep(const scalar &timestep);

    scalar previousValue() const;
    void setPreviousValue(const scalar &previous_value);

    scalar input() const;
    scalar output() const;

    virtual void compute() override;

    virtual void addInput(Block *block , const uint64_t &bind_index) override;

protected:




private:

    uint64_t _bind_index;

    bool _continuous;

    scalar _timestep;
    scalar _previous_value;


}; //class Differentiator


} //namespace Control

#endif // DIFFERENTIATOR_H
