#include "MatrixNN_global.h"

namespace MNN {


ste::Matrix<scalar> sigmoid(const ste::Matrix<scalar> &weights , const ste::Matrix<scalar> &inputs , ste::Matrix<scalar> &biases){
    return MNN::sigmoid((weights * inputs) += biases);
}
ste::Matrix<scalar> sigmoid(const ste::Matrix<scalar> &arg){
    return ste::Matrix<scalar>(arg).transform([](const scalar &value){return (scalar(1))/(scalar(1)+std::exp(-value));});
}

ste::Matrix<scalar>& sigmoid_r(ste::Matrix<scalar> &arg){
    return arg.transform([](const scalar &value){return (scalar(1))/(scalar(1)+std::exp(-value));});
}


                        /*++++++++++++++++++++++++++++++*/

ste::Matrix<scalar> sigmoid_derivative(const ste::Matrix<scalar> &arg){

    return ste::Matrix<scalar>(arg).transform([](const scalar &value){
        const scalar sig = (scalar(1))/(scalar(1)+std::exp(-value));
        return  sig*(scalar(1) - sig);
    });

}

ste::Matrix<scalar>& sigmoid_derivative_r(ste::Matrix<scalar> &arg){

    return arg.transform([](const scalar &value){
        const scalar sig = (scalar(1))/(scalar(1)+std::exp(-value));
        return  sig*(scalar(1) - sig);

    });

}

/*---------------------------------------------------------------------*/

ste::Matrix<scalar> tanh(const ste::Matrix<scalar> &weights , const ste::Matrix<scalar> &inputs , ste::Matrix<scalar> &biases){
    return MNN::tanh((weights * inputs) += biases);
}

ste::Matrix<scalar> tanh(const ste::Matrix<scalar> &arg){
    return ste::Matrix<scalar>(arg).transform([](const scalar &value){return std::tanh(value);});
}


ste::Matrix<scalar>&  tanh_r(ste::Matrix<scalar> &arg){
    return arg.transform([](const scalar &value){return std::tanh(value);});
}

                        /*++++++++++++++++++++++++++++++*/

ste::Matrix<scalar> tanh_derivative(const ste::Matrix<scalar> &arg){
    return ste::Matrix<scalar>(arg).transform([](const scalar &value){return  1 - std::pow(std::tanh(value) , 2);});
}

ste::Matrix<scalar>&  tanh_derivative_r(ste::Matrix<scalar> &arg){
    return arg.transform([](const scalar &value){return  1 - std::pow(std::tanh(value) , 2);});
}

/*---------------------------------------------------------------------*/

ste::Matrix<scalar> elu(const ste::Matrix<scalar> &weights , const ste::Matrix<scalar> &inputs , ste::Matrix<scalar> &biases, const scalar &parameter){
    return MNN::elu((weights * inputs) += biases , parameter);
}
ste::Matrix<scalar> elu(const ste::Matrix<scalar> &arg , const scalar &parameter){
    return ste::Matrix<scalar>(arg).transform([parameter](const scalar &value){return (value >= 0) ? value : parameter*(std::exp(value)-1);});
}

ste::Matrix<scalar>&  elu_r(ste::Matrix<scalar> &arg , const scalar &parameter){
    return arg.transform([parameter](const scalar &value){return (value >= 0) ? value : parameter*(std::exp(value)-1);});
}

                        /*++++++++++++++++++++++++++++++*/

ste::Matrix<scalar> elu_derivative(const ste::Matrix<scalar> &arg , const scalar &parameter){
    return ste::Matrix<scalar>(arg).transform([parameter](const scalar &value){return (value >=0) ? 1 : parameter*std::exp(value);});
}

ste::Matrix<scalar>& elu_derivative_r(ste::Matrix<scalar> &arg , const scalar &parameter){
    return arg.transform([parameter](const scalar &value){return (value >=0) ? 1 : parameter*std::exp(value);});
}

/*---------------------------------------------------------------------*/

ste::Matrix<scalar> relu(const ste::Matrix<scalar> &weights , const ste::Matrix<scalar> &inputs , ste::Matrix<scalar> &biases){
    return MNN::relu((weights * inputs) += biases);
}
ste::Matrix<scalar> relu(const ste::Matrix<scalar> &arg){
    return ste::Matrix<scalar>(arg).transform([](MNN::scalar &value){return (value > 0 ? value : 0);});
}

ste::Matrix<scalar>& relu_r(ste::Matrix<scalar> &arg){
    return arg.transform([](MNN::scalar &value){return (value > 0 ? value : 0);});
}

                        /*++++++++++++++++++++++++++++++*/

ste::Matrix<scalar> relu_derivative(const ste::Matrix<scalar> &arg){
    return ste::Matrix<scalar>(arg).transform([](scalar &value){return (value > 0);}); /// ReLU derivative is not defined for x = 0. In this library, ReLU'(0) == 0.
}                                                                                      //Note : makes use of implicit cast between 'bool' and other number types.

ste::Matrix<scalar>& relu_derivative_r(ste::Matrix<scalar> &arg){
    return arg.transform([](scalar &value){return (value > 0);}); /// ReLU derivative is not defined for x = 0. In this library, ReLU'(0) == 0.
}                                                                 //Note : makes use of implicit cast between 'bool' and other number types.
/*---------------------------------------------------------------------*/


}//namespace MNN
