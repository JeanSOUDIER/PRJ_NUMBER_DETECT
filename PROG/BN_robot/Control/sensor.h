#ifndef SENSOR_H
#define SENSOR_H

#include "block.h"

#include <functional>

namespace Control {

template<class Device>
class Sensor : public Block {

public:

    Sensor(System* sys,
           Device device, std::function<std::valarray<scalar>(Device)> accessor,
           const uint64_t &output_number)
        : Block(sys , {} , output_number , 1){

        _device = device;
        _device_accessor = accessor;

    }

    Sensor(System* sys,
           Device device, std::function<std::valarray<scalar>(Device)> accessor,
           const std::valarray<scalar> &initial_conditions)
        : Block(sys , {} , initial_conditions , 1){

        _device = device;
        _device_accessor = accessor;

    }


    Device& device(){return _device;}
    std::function<std::valarray<scalar>(Device)>& accessor(){return _device_accessor;}


    void addInput(Block *block , const uint64_t &bind_index) override{(void)block; (void)bind_index;} //Do nothing for this particular instanciation of Block.

    void compute() override{
        _outputs = _device_accessor(_device);
    }

protected:

    Device _device;
    std::function<std::valarray<scalar>(Device)> _device_accessor;

};//class Sensor

} //namespace Control



#endif // SENSOR_H
