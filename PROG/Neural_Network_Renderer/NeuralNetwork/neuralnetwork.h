#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "SequenceHandler_STL/utility.h"

#include "MatrixNN_global.h"
#include "layer.h"

#include <random>

#include <time.h>

#include <vector>


#include <QFile>


#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace MNN {

class NeuralNetwork {

public:

    NeuralNetwork(const uint64_t &layers);
    NeuralNetwork(const std::vector<Layer*> &layers);
    NeuralNetwork(const scalar &learning_rate , const uint64_t &layers);
    NeuralNetwork(const scalar &learning_rate , const uint64_t &layers, const std::string &save_path);
    NeuralNetwork(const scalar &learning_rate , const std::vector<Layer *> &layers , const std::string &save_path);
    NeuralNetwork(const scalar &learning_rate , const std::string &save_path);

    virtual ~NeuralNetwork();

    std::vector<Layer*>& layers();

    virtual void create(const std::vector<uint64_t> &layers_neurons);

#ifdef MNN_TRAIN
    void train(const uint64_t &cycles , const uint64_t &batch_size , const Train &method, const std::vector<scalar> &method_parameters);                            //Train for a fixed amount of cycles
    void train(const uint64_t &cycles , const bool &endCondition , const uint64_t &batch_size , const Train &method , const std::vector<scalar> &method_parameters);  //Trains for a fixed amount of cycles, but stops if a condition is reached
#endif
    virtual long long int predict(const ste::Matrix<scalar> &input) const;        //Predicts the ouput from the input
#ifdef MNN_TRAIN
    virtual ste::Matrix<scalar> expected(const long long int &label) const =0;
#endif

    virtual bool load(const std::string &path);                             //Load euristics from a file
    virtual bool isFileValid(const std::string &path);                      //Checks if a file is a valid data source
    virtual bool save(const std::string &path, const bool &override_old);  //Saves the current state of the network in the file specified in path
    bool save(const bool &override_old = true);


protected:

    std::vector<Layer*> _layers;

    scalar _learning_rate;

    std::string _save_path;

#ifdef MNN_TRAIN
    void train_SGD(const uint64_t &epoch , const uint64_t &batch_size );
    virtual void train_SGD(const uint64_t &epoch , const bool &endCondition , const uint64_t &batch_size) =0;

    void train_MOMENTUM(const uint64_t &epoch , const uint64_t &batch_size  , const scalar &beta);
    virtual void train_MOMENTUM(const uint64_t &epoch , const bool &endCondition , const uint64_t &batch_size  , const scalar &beta) =0;

    void train_RMS(const uint64_t &epoch , const uint64_t &batch_size , const scalar &beta , const scalar &epsilon);
    virtual void train_RMS(const uint64_t &epoch , const bool &endCondition , const uint64_t &batch_size , const scalar &beta , const scalar &epsilon) =0;

    void train_ADAM(const uint64_t &epoch , const uint64_t &batch_size , const scalar &beta1  = 0.9f, const scalar &beta2  = 0.999f, const scalar &epsilon = 10e-8f);
    virtual void train_ADAM(const uint64_t &epoch , const bool &endCondition , const uint64_t &batch_size , const scalar &beta1 = 0.9f , const scalar &beta2  =0.999f, const scalar &epsilon = 10e-8f) =0;
#endif
    void clear_layers();

    virtual scalar cost(const ste::Matrix<scalar> &expected_output , const ste::Matrix<scalar> &obtained_output) const = 0;                     //Computes the cost function
    scalar loss(const ste::Matrix<scalar> &expected_output , const ste::Matrix<scalar> &obtained_output) const;

};//class NeuralNetwork

}//namespace MNN



#endif // NEURALNETWORK_H
