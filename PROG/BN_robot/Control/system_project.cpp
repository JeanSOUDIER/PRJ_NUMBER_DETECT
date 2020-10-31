#include "system_project.h"

namespace Control {
/**************************************************************************/
System_project::System_project(const uint64_t Ts_ms, MobileBase *robot) : System(Ts_ms){

    generator = new Generator_Project(this , robot , {0 , 0} , 10 , 5);
    feedback_sensor = new Sensor<MobileBase*>(this , &robot , &MobileBase::currentPos_helper , {0 , 0 , 0});

    d_x_in = new Differentiator(this , generator , 0 , Ts() , 0 , 1 , true);
    d_y_in = new Differentiator(this , generator , 1 , Ts() , 0 , 1 , true);

    theta_in = new FunctionBlock(this , {d_x_in , d_y_in} , {0 , 0});

    feedback_x_comparator = new Adder(this , {generator , feedback_sensor} , {0 , 0} , {Adder::POLARITY::PLUS , Adder::POLARITY::MINUS} , 1);
    feedback_y_comparator = new Adder(this , {generator , feedback_sensor} , {1 , 1} , {Adder::POLARITY::PLUS , Adder::POLARITY::MINUS} , 1);
    feedback_theta_comparator = new Adder(this , {theta_in , feedback_sensor} , {0 , 2} , {Adder::POLARITY::PLUS , Adder::POLARITY::MINUS} , 1);;


    to_parameters = new FunctionBlock(this , {feedback_x_comparator , feedback_y_comparator , feedback_theta_comparator , feedback_sensor} , {0 , 0 , 0 , 2});


    to_v_w = new FunctionBlock(this , {to_parameters} , {0 , 1 , 2});



    std::function<std::valarray<scalar>(std::valarray<scalar>)> atan_2_lambda = [=](const std::valarray<scalar>&){
        return std::valarray<scalar> {static_cast<scalar>(std::fmod(std::atan2(d_y_in->output() , d_x_in->output()) , T_PI_LD))};
    };


    std::function<std::valarray<scalar>(std::valarray<scalar>)> to_parameters_lambda = [=](const std::valarray<scalar>&){

        const scalar alpha = static_cast<scalar>(std::fmod(std::atan2(feedback_y_comparator->output() ,feedback_x_comparator->output() ) , T_PI_LD)) - feedback_theta_comparator->output();

        return std::valarray<scalar>({
                                       std::sqrt(std::pow(feedback_x_comparator->output() , 2) + std::pow(feedback_y_comparator->output() , 2)),
                                       alpha,
                                       feedback_sensor->output(3) - alpha
                                     });
    };

    std::function<std::valarray<scalar>(std::valarray<scalar>)> to_v_w_lambda = [=](const std::valarray<scalar>&){

        constexpr scalar k_lambda = -5.5;
        constexpr scalar k_alpha = -220;
        constexpr scalar k_beta = -20;


        return std::valarray<scalar>({
                                      -k_lambda * to_parameters->output(0) ,
                                      - k_alpha*to_parameters->output(1) - k_beta*to_parameters->output(2)
                                     });
    };

    theta_in->setFunction(atan_2_lambda);
    to_parameters->setFunction(to_parameters_lambda);
    to_v_w->setFunction(to_v_w_lambda);

    ///TODO ??????? Convert (v ; w) to <something>


}

System_project::~System_project(){


    delete generator;
    delete feedback_sensor;

    delete d_x_in;
    delete d_y_in;

    delete theta_in;



    delete feedback_x_comparator;
    delete feedback_y_comparator;
    delete feedback_theta_comparator;


    delete to_parameters;


    delete to_v_w;

    _blocks.clear();

}


void System_project::compute(){


    ///N.B.: Due to how the constructor is made, is was not necessary to override this function.
    /// Indeed, all blocks were declared in the correct order of computation. This implies that is was not necessary to declare them as attribute of the class.
    /// However, for documentation purposes, this is let as such.

    generator->compute();
    feedback_sensor->compute();

    d_x_in->compute();
    d_y_in->compute();
    theta_in->compute();

    feedback_x_comparator->compute();
    feedback_y_comparator->compute();
    feedback_theta_comparator->compute();

    to_parameters->compute();
    to_v_w->compute();


}

scalar System_project::v() const{return to_v_w->output(0);}
scalar System_project::w() const{return to_v_w->output(1);}

/**************************************************************************/
} //namespace Control
