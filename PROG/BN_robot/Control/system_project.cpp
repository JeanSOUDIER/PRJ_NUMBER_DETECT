#include "system_project.h"

namespace Control {
/**************************************************************************/
System_project::System_project(const uint64_t Ts_ms, MobileBase *robot, std::valarray<scalar> initial_conditions, const scalar &line_length, const scalar &arc_diameter) : System(Ts_ms){

    generator = new Generator_Project(this , robot , initial_conditions, line_length , arc_diameter);
    feedback_sensor = new Sensor<MobileBase*>(this , robot , &MobileBase::currentPos_helper_meter , {0 , 0 , 0});

    d_x_in = new Differentiator(this , generator , 0 , Ts() , 0 , 1 , false);
    d_y_in = new Differentiator(this , generator , 1 , Ts() , 0 , 1 , false);

    theta_in = new FunctionBlock(this , {d_x_in , d_y_in} , {0 , 0});

    feedback_x_comparator = new Adder(this , {generator , feedback_sensor} , {0 , 0} , {Adder::POLARITY::PLUS , Adder::POLARITY::MINUS} , 1);
    feedback_y_comparator = new Adder(this , {generator , feedback_sensor} , {1 , 1} , {Adder::POLARITY::PLUS , Adder::POLARITY::MINUS} , 1);
    feedback_theta_comparator = new Adder(this , {theta_in , feedback_sensor} , {0 , 2} , {Adder::POLARITY::PLUS , Adder::POLARITY::MINUS} , 1);;

    to_parameters = new FunctionBlock(this , {feedback_x_comparator , feedback_y_comparator , feedback_theta_comparator , feedback_sensor} , {0 , 0 , 0 , 2});

    to_v_w = new FunctionBlock(this , {to_parameters} , {0});
    w_integrator = new Integrator(this , to_v_w , 1 , Ts() , 0 , 0 , 1 , false);
    to_vx_vy = new FunctionBlock(this , {to_v_w , w_integrator} , {0 , 1});


    std::function<std::valarray<scalar>(std::valarray<scalar>)> atan_2_lambda = [=](const std::valarray<scalar>&){
        return std::valarray<scalar> {static_cast<scalar>(std::fmod(std::atan2(d_y_in->output() , d_x_in->output()) , T_PI_LD))};
    };


    std::function<std::valarray<scalar>(std::valarray<scalar>)> to_parameters_lambda = [=](const std::valarray<scalar>&){

        const scalar alpha = static_cast<scalar>(std::fmod(std::atan2(feedback_y_comparator->output() ,feedback_x_comparator->output() ) , T_PI_LD)) - feedback_sensor->output(2);

        return std::valarray<scalar>({
                                       std::sqrt(std::pow(feedback_x_comparator->output() , 2) + std::pow(feedback_y_comparator->output() , 2)),
                                       alpha,
                                       feedback_theta_comparator->output() - alpha
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

    std::function<std::valarray<scalar>(std::valarray<scalar>)> to_vx_vy_lambda = [=](const std::valarray<scalar>&){

        return std::valarray<scalar>({
                                       to_v_w->output(0) * std::cos(w_integrator->output()),
                                       to_v_w->output(1) * std::sin(w_integrator->output())
                                     });
    };

    theta_in->setFunction(atan_2_lambda);
    to_parameters->setFunction(to_parameters_lambda);
    to_v_w->setFunction(to_v_w_lambda);
    to_vx_vy->setFunction(to_vx_vy_lambda);
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
    delete w_integrator;
    delete to_vx_vy;

    _blocks.clear();

}


void System_project::compute(){


    ///N.B.: Due to how the constructor is made, is was not necessary to override this function.
    /// Indeed, all blocks were declared in the correct order of computation. This implies that is was not necessary to declare them as attribute of the class.
    /// However, for documentation purposes, this is let as such.

    //std::cout << "System compute called.\n" << std::endl;

    generator->compute();
    //std::cout << "xr: " << generator->x() << " yr: " << generator->y() << "\n";

    feedback_sensor->compute();
    //std::cout << "x (feedback): " << feedback_sensor->output(0) << " y (feedback): " << feedback_sensor->output(1)  << " theta (feedback): "<< feedback_sensor->output(2) << "\n";

    d_x_in->compute();
    //std::cout << "d_x_r: " << d_x_in->output();

    d_y_in->compute();
    //std::cout << "d_y_r: " << d_y_in->output();

    theta_in->compute();
    //std::cout << "theta_in: " << theta_in->output(0);

    feedback_x_comparator->compute();
    //std::cout << "feedback_x_comparator: " << feedback_x_comparator->output() << "\n";

    feedback_y_comparator->compute();
    //std::cout << "feedback_y_comparator: " << feedback_y_comparator->output() << "\n";;

    feedback_theta_comparator->compute();
    //std::cout << "feedback_theta_comparator: " << feedback_theta_comparator->output() << "\n";;

    to_parameters->compute();
    //std::cout << "lambda: " << to_parameters->output(0) << " alpha: " << to_parameters->output(1) << " beta: " << to_parameters->output(2) << "\n";

    to_v_w->compute();
    //std::cout << "v: " << to_v_w->output(0) << " w: " << to_v_w->output(1)  << "\n";

    w_integrator->compute();
    //std::cout << "w_integrator: " << w_integrator->output() << "\n";


    to_vx_vy->compute();
    //std::cout << "vx: " << to_vx_vy->output(0) << " vy: " << to_vx_vy->output(1) << "\n"<<std::endl;


}

scalar System_project::vx() const{return to_vx_vy->output(0);}
scalar System_project::vy() const{return to_vx_vy->output(1);}

/**************************************************************************/
} //namespace Control
