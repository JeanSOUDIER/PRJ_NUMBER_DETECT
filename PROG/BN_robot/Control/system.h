#ifndef SYSTEM_H
#define SYSTEM_H

#include "Control_global.h"

#include <chrono>

#include <ratio>

#include <vector>

#include <thread>

namespace Control {

class Block;

class System{

    friend class Block;

public:

    explicit System(const uint64_t Ts_ms = 1);
    ~System();

    std::vector<Block*> blocks() const;

    void start();
    void stop();
    void setRunning(const bool run);

    bool isRunning();


    uint64_t Ts() const;
    void setSampleTime(const uint64_t &ms_value);


protected:

    void addBlock(Block *block);
    virtual void compute();
    static void compute_helper(void *context);

    uint64_t _Ts_ms;

    bool _isRunning;

    std::vector<Block*> _blocks;

private:


};//class System

}//namespace Control

#endif // SYSTEM_H
