#include "MatrixNN_global.h"

namespace MNN {


ste::Matrix<scalar> sigmoid(const ste::Matrix<scalar> &weights , const ste::Matrix<scalar> &inputs , ste::Matrix<scalar> &biases){
    return MNN::sigmoid((weights * inputs) + biases);
}
ste::Matrix<scalar> sigmoid(const ste::Matrix<scalar> &arg){

    ste::Matrix<scalar> result = arg;

    for(uint64_t element = 0 ; element < result.rows() * result.columns() ; element++){
        result.replace(element , (1.)/(1.+std::exp(-result.at(element))));
    }

    return result;

}


ste::Matrix<scalar> sigmoid_derivative(const ste::Matrix<scalar> &arg){


    ste::Matrix<scalar> result = arg;

    for(uint64_t element = 0 ; element < result.rows() * result.columns() ; element++){

        const scalar sig = (1.)/(1.+std::exp(-result.at(element))); //Do not do the computation twice
        result.replace(element , sig*(scalar(1) - sig));
    }

    return result;


}

ste::Matrix<scalar> tanh(const ste::Matrix<scalar> &weights , const ste::Matrix<scalar> &inputs , ste::Matrix<scalar> &biases){
    return MNN::tanh((weights * inputs) + biases);
}
ste::Matrix<scalar> tanh(const ste::Matrix<scalar> &arg){

    ste::Matrix<scalar> result = arg;

    for(uint64_t element = 0 ; element < result.rows() * result.columns() ; element++){
        result.replace(element , std::tanh(result.at(element)));
    }

    return result;



}


ste::Matrix<scalar> tanh_derivative(const ste::Matrix<scalar> &arg){

    ste::Matrix<scalar> result = arg;

    for(uint64_t element = 0 ; element < result.rows() * result.columns() ; element++){
        result.replace(element , 1 - std::pow(std::tanh(result.at(element)) , 2));
    }

    return result;


}

ste::Matrix<scalar> elu(const ste::Matrix<scalar> &weights , const ste::Matrix<scalar> &inputs , ste::Matrix<scalar> &biases){
    return elu((weights * inputs) + biases);
}
ste::Matrix<scalar> elu(const ste::Matrix<scalar> &arg){

    ste::Matrix<scalar> result = arg;

    for(uint64_t element = 0 ; element < result.rows() * result.columns() ; element++){

        result.replace(element ,
                       (result.at(element) >= 0) ? result.at(element) : 0.2*(std::exp(result.at(element))-1));
    }

    return result;

}

ste::Matrix<scalar> elu_derivative(const ste::Matrix<scalar> &arg){


    ste::Matrix<scalar> result = arg;

    for(uint64_t element = 0 ; element < result.rows() * result.columns() ; element++){

        result.replace(element ,
                       (result.at(element) >= 0) ? result.at(element) : 0.2*std::exp(result.at(element)));
    }

    return result;

}


}//namespace MNN
