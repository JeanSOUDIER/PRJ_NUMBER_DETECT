#ifndef SYSTEM_PROJECT_H
#define SYSTEM_PROJECT_H


#include "adder.h"

#include "differentiator.h"

#include "functionblock.h"

#include "integrator.h"

#include "generator_project.h"

#include "system.h"

#include "sensor.h"


#include "Robot/MobileBase.hpp"

namespace Control {

class System_project : public System {

public:

    System_project(const uint64_t Ts_ms = 1 , MobileBase* robot= nullptr);
    ~System_project();

    scalar v() const;
    scalar w() const;

protected:

    void compute() override;


private:

    Generator_Project *generator;
    Sensor<MobileBase*> *feedback_sensor;

    Differentiator *d_x_in;
    Differentiator *d_y_in;

    FunctionBlock *theta_in;



    Adder *feedback_x_comparator;
    Adder *feedback_y_comparator;
    Adder *feedback_theta_comparator;


    FunctionBlock *to_parameters;


    FunctionBlock *to_v_w;

};

}

#endif // SYSTEM_PROJECT_H
