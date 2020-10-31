#include "system.h"
#include "block.h"

namespace Control {
/**************************************************************************/
System::System(const uint64_t Ts_ms){
    _Ts_ms = (Ts_ms>0) ? Ts_ms : 1;
    _isRunning = false;
}

System::~System(){

    //Clear memory
    if(blocks().size() > 0){

        auto st = _blocks.begin();
        auto ed = _blocks.end();

        while (st != ed) {
            delete *st;
            ++st;
        }

    }

}


void System::addBlock(Block *block){_blocks.push_back(block);}


void System::start(){

    if(_isRunning){return;}
    _isRunning = true;

    std::thread t(
    [=]() { //Lambda to describe action while running

            while(isRunning()){
                if(!_isRunning){return;} //Return if stopped here
                std::this_thread::sleep_for(std::chrono::milliseconds(_Ts_ms)); //Wait
                if(!_isRunning){return;} //Return if stopped here
                compute();
            }

            return; //Exit thread
    }
    );
    t.detach();

}
void System::stop(){

    if(!_isRunning){return;}
    _isRunning = false;

}




void System::compute(){
    for(Block* block : _blocks){block->compute();}
}

void System::compute_helper(void *context){static_cast<System*>(context)->compute();}




void System::setRunning(const bool run){

    if(run){start();}
    else{stop();}

}

bool System::isRunning(){return _isRunning;}


uint64_t System::Ts() const{return _Ts_ms;}
void System::setSampleTime(const uint64_t &ms_value){_Ts_ms = (ms_value>0) ? ms_value : 1;}

std::vector<Block*> System::blocks() const {return _blocks;}
/**************************************************************************/
} //namespace Control

