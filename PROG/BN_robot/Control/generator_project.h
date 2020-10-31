#ifndef GENERATOR_PROJECT_H
#define GENERATOR_PROJECT_H

#include <cmath>

#include "generator.h"

namespace Control {

class System_project;

class Generator_Project : public Generator{

    friend class System;
    friend class System_project;

public:

    Generator_Project(System_project* sys , std::valarray<scalar> initial_conditions = {0 , 0}, const scalar &line_length = 10 , const scalar &arc_diameter = 5);
    ~Generator_Project() override;

    void compute() override;

protected:

    scalar &x();
    scalar &y();

    scalar line_length();
    scalar arc_diameter();

private:

    scalar _line_length;
    scalar _arc_diameter;


}; //class Generator_Project

} //namespace Control


#endif // GENERATOR_PROJECT_H
