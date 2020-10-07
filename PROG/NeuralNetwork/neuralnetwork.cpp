#include "neuralnetwork.h"

namespace BasicNN {
/*----------------------------------------------------------------------------*/

NeuralNetwork::NeuralNetwork(const BasicNN::scalar &learning_rate, const uint64_t &layer_number){

    _learning_rate = learning_rate;
    _layers.reserve(layer_number);
    _save_path = "C:/";

}

NeuralNetwork::NeuralNetwork(const BasicNN::scalar &learning_rate, const uint64_t &layer_number, const std::string &save_path) : NeuralNetwork(learning_rate , layer_number){

    _save_path = save_path;

}


NeuralNetwork::NeuralNetwork(const BasicNN::scalar &learning_rate , const  std::vector<Layer> &layers , const std::string &save_path){

    _learning_rate = learning_rate;
    _layers = layers;
    _save_path = save_path;

}
NeuralNetwork::NeuralNetwork(const BasicNN::scalar &learning_rate , const std::string &save_path){

    _learning_rate = learning_rate;
    _layers.reserve(4);
    _save_path = save_path;

}

NeuralNetwork::~NeuralNetwork(){}
/*----------------------------------------------------------------------------*/
}

