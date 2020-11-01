#ifndef ADDER_H
#define ADDER_H

#include "block.h"


namespace Control {

class Adder : public Block {

    friend class System;

public:

    enum class POLARITY{
        PLUS = 1,
        P = PLUS,
        ADD = PLUS,
        POS = PLUS,

        MINUS = -1,
        M = MINUS,
        SUB = MINUS,
        NEG = MINUS

    };

    Adder(System *sys, const std::vector<Block*> &inputs , const std::vector<uint64_t> &bind_indexes , const std::vector<POLARITY> &polarities , const scalar &gain = 1);
    ~Adder() override;

    scalar output() const;
    std::vector<POLARITY> polarities() const;
    std::vector<uint64_t> bind_indexes() const;

    virtual void addInput(Block * block , const uint64_t &bind_index) override;
    void addInput(Block *block , const uint64_t &bind_index ,  const POLARITY &polarity);

    void setPolarity(const uint64_t &index , const POLARITY &polarity);

    void compute() override;

protected:




private:

    std::vector<uint64_t> _bind_indexes;
    std::vector<POLARITY> _polarities;



}; //class Adder

} //namespace Control

#endif // ADDER_H
