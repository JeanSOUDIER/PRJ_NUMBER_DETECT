#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "NeuralNetwork_global.h"

#include "neuron.h"

#include "layer.h"




#include <string>

#include <vector>

namespace BasicNN {
/*----------------------------------------------------------------------------*/

class NEURALNETWORK_EXPORT NeuralNetwork{

public:

    NeuralNetwork(const BasicNN::scalar &learning_rate , const uint64_t &layer_number);
    NeuralNetwork(const BasicNN::scalar &learning_rate , const uint64_t &layer_number , const std::string &save_path);
    NeuralNetwork(const BasicNN::scalar &learning_rate , const  std::vector<Layer> &layers , const std::string &save_path);
    NeuralNetwork(const BasicNN::scalar &learning_rate , const std::string &save_path);
    virtual ~NeuralNetwork() = 0;

protected:

    std::vector<Layer> _layers;
    BasicNN::scalar _learning_rate;
    std::string _save_path;


    virtual void create(const uint64_t &layer_number , const uint64_t &layers_cells , const BasicNN::scalar &neuron_threshold , const BasicNN::scalar &neuron_offset , const std::vector<BasicNN::scalar> &neuron_weights) =0;
    virtual void create(const uint64_t &layer_number , const uint64_t &layers_cells) =0;

    virtual std::vector<Layer> layers() = 0;
    virtual void train(const uint64_t &cycles) = 0;                             //Train for a fixed amount of cycles
    virtual void train(const bool &endCondition) = 0;                           //Trains until a condition is reached
    virtual void train(const uint64_t &cycles , const bool &endCondition) = 0;  //Trains for a fixed amount of cycles, but stops if a condition is reached

    virtual bool load(const std::string &path) = 0;                             //Load euristics from a file
    virtual bool load(const QString &path) = 0;                                 //Load euristics from a file
    virtual bool isFileValid(const std::string &path) = 0;                      //Checks if a file is a valid data source
    virtual bool isFileValid(const QString &path) = 0;                          //Checks if a file is a valid data source
    virtual bool save(const std::string &path , const bool &override_old) = 0;  //Saves the current state of the network in the file specified in path
    virtual bool save(const QString &path , const bool &override_old) =0;       //Saves the current state of the network in the file specified in path


    virtual void addLayer(const Layer&) = 0;                                    //Adds a Layer
    virtual void addLayer(const std::vector<Neuron> &) = 0;                     //Adds a Layer create from a vector of Neurons
    virtual void addLayer(const uint64_t &cell_number) = 0;                     //Adds a Layer with a preallocated vector of Neurons

    virtual void error(const bool &recompute) =0;                               //Computes the error
    virtual unsigned char predict() = 0;                                        //Predicts the ouput from the input

private:

};

/*----------------------------------------------------------------------------*/
}


#endif // NEURALNETWORK_H
