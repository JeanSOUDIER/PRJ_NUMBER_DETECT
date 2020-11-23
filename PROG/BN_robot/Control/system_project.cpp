#include "system_project.h"

namespace Control {
/**************************************************************************/
System_project::System_project(const uint64_t Ts_ms, MobileBase *robot, std::valarray<scalar> initial_conditions, const scalar &line_length, const scalar &arc_diameter) : System(Ts_ms){

    generator = new Generator_Project(this , robot , initial_conditions, line_length , arc_diameter);
    feedback_sensor = new Sensor<MobileBase*>(this , robot , &MobileBase::currentPos_helper_meter , initial_conditions);

    d_x_in = new Differentiator(this , generator , 0 , Ts()/1000., 0 , 1 , false);
    d_y_in = new Differentiator(this , generator , 1 , Ts()/1000., 0 , 1 , false);

    theta_in = new FunctionBlock(this , {d_x_in , d_y_in} , {0 , 0});

    feedback_x_comparator = new Adder(this , {generator , feedback_sensor} , {0 , 0} , {Adder::POLARITY::PLUS , Adder::POLARITY::MINUS} , 1);
    feedback_y_comparator = new Adder(this , {generator , feedback_sensor} , {1 , 1} , {Adder::POLARITY::PLUS , Adder::POLARITY::MINUS} , 1);
    feedback_theta_comparator = new Adder(this , {theta_in , feedback_sensor} , {0 , 2} , {Adder::POLARITY::PLUS , Adder::POLARITY::MINUS} , 1);;

    to_parameters = new FunctionBlock(this , {feedback_x_comparator , feedback_y_comparator , feedback_theta_comparator , feedback_sensor} , {0 , 0 , 0 , 2});

    to_v_w = new FunctionBlock(this , {to_parameters} , {0});
    w_integrator = new Integrator(this , to_v_w , 1 , Ts()/1000., -M_PI/2 , -M_PI/2 , 1 , true);
    to_vx_vy = new FunctionBlock(this , {to_v_w , w_integrator} , {0 , 1});
    

    vx_intgrator = new Integrator(this , to_vx_vy , 0 , Ts()/1000., 0 , 0 , 1 , true);
    vy_intgrator = new Integrator(this , to_vx_vy , 1 , Ts()/1000., 0 , 0 , 1 , true);
    to_vr_vl = new FunctionBlock(this, {to_v_w, w_integrator, vy_intgrator}, {0, 1, 1});

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

        constexpr scalar k_lambda = -2.1;
        constexpr scalar k_alpha = -50;
        constexpr scalar k_beta = 1;

        return std::valarray<scalar>({
                                      -k_lambda * to_parameters->output(0) ,
                                      - k_alpha*to_parameters->output(1) - k_beta*to_parameters->output(2)
                                     });
    };

    std::function<std::valarray<scalar>(std::valarray<scalar>)> to_vx_vy_lambda = [=](const std::valarray<scalar>&){

        return std::valarray<scalar>({
                                       to_v_w->output(0) * std::cos(w_integrator->output()),
                                       to_v_w->output(0) * std::sin(w_integrator->output())
                                     });
    };

    std::function<std::valarray<scalar>(std::valarray<scalar>)> to_vr_vl_lambda = [=](const std::valarray<scalar>&){

        return std::valarray<scalar>({
                                       80*to_v_w->output(0)*(1 + 1.45*std::cos(w_integrator->output())*(Utility::sign(vy_intgrator->output()-line_length/2))),
                                       80*to_v_w->output(0)*(1 - 1.45*std::cos(w_integrator->output())*(Utility::sign(vy_intgrator->output()-line_length/2)))
                                     });
    };

    theta_in->setFunction(atan_2_lambda);
    to_parameters->setFunction(to_parameters_lambda);
    to_v_w->setFunction(to_v_w_lambda);
    to_vx_vy->setFunction(to_vx_vy_lambda);
    to_vr_vl->setFunction(to_vr_vl_lambda);
    
    feedback_sensor->compute();
    std::cout << "x (feedback): " << feedback_sensor->output(0) << " y (feedback): " << feedback_sensor->output(1)  << " theta (feedback): "<< feedback_sensor->output(2) << "\n";

    d_x_in->compute();
    std::cout << "d_x_r: " << d_x_in->output();

    d_y_in->compute();
    std::cout << " d_y_r: " << d_y_in->output();

    theta_in->compute();
    std::cout << " theta_in: " << theta_in->output(0) << "\n";

    feedback_x_comparator->compute();
    std::cout << "feedback_x_comparator: " << feedback_x_comparator->output();

    feedback_y_comparator->compute();
    std::cout << " feedback_y_comparator: " << feedback_y_comparator->output();

    feedback_theta_comparator->compute();
    std::cout << " feedback_theta_comparator: " << feedback_theta_comparator->output() << "\n";;

    to_parameters->compute();
    std::cout << "lambda: " << to_parameters->output(0) << " alpha: " << to_parameters->output(1) << " beta: " << to_parameters->output(2) << "\n";

    to_v_w->compute();
    std::cout << "v: " << to_v_w->output(0) << " w: " << to_v_w->output(1)  << "\n";
    
    std::cout << "w_integrator: " << w_integrator->output() << "\n";

    to_vx_vy->compute();
    std::cout << "vx: " << to_vx_vy->output(0) << " vy: " << to_vx_vy->output(1) <<std::endl;

    to_vr_vl->compute();
    std::cout << "vr: " << to_vr_vl->output(0) << " vl: " << to_vr_vl->output(1) <<std::endl;

    vx_intgrator->compute();
    std::cout << "vx_integrator: " << vx_intgrator->output() << "\n";

    vy_intgrator->compute();
    std::cout << "vy_integrator: " << vy_intgrator->output() << "\n";
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

    delete to_vr_vl;
    delete vx_intgrator;
    delete vy_intgrator;

    _blocks.clear();

}


void System_project::compute(){


    ///N.B.: Due to how the constructor is made, is was not necessary to override this function.
    /// Indeed, all blocks were declared in the correct order of computation. This implies that is was not necessary to declare them as attribute of the class.
    /// However, for documentation purposes, this is let as such.

    std::cout << "System compute called.\n" << std::endl;

    generator->compute();
    std::cout << "xr: " << generator->x() << " yr: " << generator->y() << "\n";

    feedback_sensor->compute();
    std::cout << "x (feedback): " << feedback_sensor->output(0) << " y (feedback): " << feedback_sensor->output(1)  << " theta (feedback): "<< feedback_sensor->output(2) << "\n";

    d_x_in->compute();
    std::cout << "d_x_r: " << d_x_in->output();

    d_y_in->compute();
    std::cout << " d_y_r: " << d_y_in->output();

    theta_in->compute();
    std::cout << " theta_in: " << theta_in->output(0) << "\n";

    feedback_x_comparator->compute();
    std::cout << "feedback_x_comparator: " << feedback_x_comparator->output();

    feedback_y_comparator->compute();
    std::cout << " feedback_y_comparator: " << feedback_y_comparator->output();

    feedback_theta_comparator->compute();
    std::cout << " feedback_theta_comparator: " << feedback_theta_comparator->output() << "\n";

    to_parameters->compute();
    std::cout << "lambda: " << to_parameters->output(0) << " alpha: " << to_parameters->output(1) << " beta: " << to_parameters->output(2) << "\n";

    to_v_w->compute();
    std::cout << "v: " << to_v_w->output(0) << " w: " << to_v_w->output(1)  << "\n";
    
    std::cout << "w_integrator: " << w_integrator->output() << "\n";

    to_vx_vy->compute();
    std::cout << "vx: " << to_vx_vy->output(0) << " vy: " << to_vx_vy->output(1) <<std::endl;

    to_vr_vl->compute();
    std::cout << "vr: " << to_vr_vl->output(0) << " vl: " << to_vr_vl->output(1) <<std::endl;

    vx_intgrator->compute();
    std::cout << "vx_integrator: " << vx_intgrator->output() << "\n";

    vy_intgrator->compute();
    std::cout << "vy_integrator: " << vy_intgrator->output() << "\n";
    
    w_integrator->compute();
    std::cout << "w_integrator: " << w_integrator->output() << "\n";

}

scalar System_project::vr() const{return to_vr_vl->output(0);}
scalar System_project::vl() const{return to_vr_vl->output(1);}
std::valarray<scalar> System_project::coord() const{return {vx_intgrator->output()*1000, vy_intgrator->output()*1000, w_integrator->output()};}

/**************************************************************************/
} //namespace Control
