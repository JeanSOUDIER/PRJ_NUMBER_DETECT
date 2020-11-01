#ifndef DEMULTIPLEXER_H
#define DEMULTIPLEXER_H

#include "block.h"

namespace Control {

class Demultiplexer : public Block {

        friend class System;

public:

    Demultiplexer(System *sys , Block *input ,
                  const uint64_t &bind_index,
                  const uint64_t &output_number,
                  const scalar &gain = 1);

    Demultiplexer(System *sys, Block *input ,
                  const uint64_t &bind_index,
                  const uint64_t &output_number,
                  const uint64_t &initial_index,
                  const scalar &gain = 1);

    ~Demultiplexer() override;

    void link(const uint64_t &index);


    scalar input() const;
    scalar output() const;

    virtual void compute() override;

protected:



private:

    uint64_t _bind_index;
    uint64_t _current_output;

    Block* _input_block;


};//class Demultiplexer

}//namespace Control


#endif // DEMULTIPLEXER_H
