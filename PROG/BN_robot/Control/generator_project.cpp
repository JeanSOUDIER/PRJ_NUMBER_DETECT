#include "generator_project.h"
#include "system_project.h"

namespace Control {
/**************************************************************************/

Generator_Project::Generator_Project(System_project *sys , MobileBase *robot, std::valarray<scalar> initial_conditions, const scalar &line_length, const scalar &arc_diameter)
    :Generator(sys , initial_conditions){

    _line_length = line_length;
    _arc_diameter = arc_diameter;
    _robot = robot;

}

Generator_Project::~Generator_Project(){}


void Generator_Project::compute(){


    const bool is_on_part_1_flag = (x() == 0 && y() < line_length());
    const bool is_on_part_2_flag = (x() < arc_diameter() && y() >= line_length());
    const bool is_on_part_3_flag = (x() > arc_diameter() && y() >= 0);
    const bool is_on_part_4_flag = (!is_on_part_1_flag &&
                                    !is_on_part_2_flag &&
                                    !is_on_part_3_flag);

    const scalar v = _robot->GetSpeedCons();

    x() = is_on_part_1_flag * (0)+
          is_on_part_2_flag * (x() + Ts()*v/1000.)+
          is_on_part_3_flag * (arc_diameter())+
          is_on_part_4_flag * ((x() - Ts()*v/1000. > 0)*(x() - Ts()*v/1000.));

    y() = is_on_part_1_flag * (y() + Ts()*v/1000.)+
          is_on_part_2_flag * (line_length() + (arc_diameter()/2)*std::sin(PI_LD*x()/arc_diameter()))+
          is_on_part_3_flag * (y() - Ts()*v/1000.)+
          is_on_part_4_flag * (- (arc_diameter()/2)*std::sin(PI_LD*x()/arc_diameter()));

}


scalar& Generator_Project::x(){return _outputs[0];}
scalar& Generator_Project::y(){return _outputs[1];}
scalar Generator_Project::line_length(){return _line_length;}
scalar Generator_Project::arc_diameter(){return _arc_diameter;}
/**************************************************************************/
} //namespace Control

