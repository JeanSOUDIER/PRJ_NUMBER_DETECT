#ifndef SYSTEM_PROJECT_H
#define SYSTEM_PROJECT_H


#include "adder.h"

#include "differentiator.h"

#include "functionblock.h"

#include "integrator.h"

#include "generator_project.h"

#include "system.h"

#include "sensor.h"


#include "../Robot/MobileBase.hpp"

namespace Control {

class System_project : public System {

public:

    System_project(const uint64_t Ts_ms = 1 , MobileBase* robot= nullptr , std::valarray<scalar> initial_conditions = {0 , 0}, const scalar &line_length = 10 , const scalar &arc_diameter = 5);
    ~System_project();

    scalar vx() const;
    scalar vy() const;
    void compute() override;

protected:



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
    Integrator *w_integrator;
    FunctionBlock *to_vx_vy;

};

}

#endif // SYSTEM_PROJECT_H
