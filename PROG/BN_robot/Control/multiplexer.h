#ifndef MULTIPLEXER_H
#define MULTIPLEXER_H

#include "block.h"
namespace Control {

class Multiplexer : public Block {

        friend class System;

public:

    Multiplexer(System *sys ,
                const std::vector<Block*> &inputs ,
                const std::vector<uint64_t> &bind_indexes ,
                const uint64_t &initial_out_index = 0,
                const scalar &gain = 1);


    ~Multiplexer() override;

    void link(const uint64_t &index);


    scalar active_input() const;
    scalar output() const;

    virtual void compute() override;

protected:



private:

    std::vector<uint64_t> _bind_indexes;

    uint64_t _out_selector;


}; //class Multiplexer

} //namespace Control

#endif // MULTIPLEXER_H
