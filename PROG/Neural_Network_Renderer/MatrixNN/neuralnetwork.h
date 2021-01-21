#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "SequenceHandler_STL/utility.h"

#include "MatrixNN_global.h"
#include "layer.h"

#ifdef MNN_TRAIN
#include "dataset_handler.h"

#include <random>
#include <time.h>
#endif


#include <vector>


#include <QFile>


#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace MNN {

class NeuralNetwork {

public:

    NeuralNetwork(const uint64_t &layers);
    NeuralNetwork(const std::vector<Layer*> &layers);

#ifdef MNN_TRAIN
    NeuralNetwork(const scalar &learning_rate , const uint64_t &layers);
    NeuralNetwork(const scalar &learning_rate , const uint64_t &layers, const std::string &save_path);
    NeuralNetwork(const scalar &learning_rate , const std::vector<Layer *> &layers , const std::string &save_path);
    NeuralNetwork(const uint64_t &layers , const std::string &save_path);
#endif
    virtual ~NeuralNetwork();

    std::vector<Layer*>& layers();



#ifdef MNN_TRAIN

    virtual void create(const std::vector<uint64_t> &layers_neurons);

    void train(const uint64_t &cycles , const uint64_t &batch_size , const Train &method, const std::vector<scalar> &method_parameters);                            //Train for a fixed amount of cycles
    void train(const uint64_t &cycles , const bool &endCondition , const uint64_t &batch_size , const Train &method , const std::vector<scalar> &method_parameters);  //Trains for a fixed amount of cycles, but stops if a condition is reached

    virtual ste::Matrix<scalar> expected(const long long int &label) const =0;

    virtual bool save(const std::string &path, const bool &override_old);  //Saves the current state of the network in the file specified in path
    bool save(const bool &override_old = true);

#endif
    virtual long long int predict(const ste::Matrix<scalar> &input) const;        //Predicts the ouput from the input

    virtual bool load(const std::string &path);                             //Load euristics from a file
    virtual bool isFileValid(const std::string &path);                      //Checks if a file is a valid data source



protected:

    std::vector<Layer*> _layers;
    void clear_layers();

#ifdef MNN_TRAIN
    std::string _save_path;

    scalar _learning_rate;

    Dataset_Handler *dataset_trainer;
    Dataset_Handler *dataset_tester;

    void train_SGD(const uint64_t &epoch , const uint64_t &batch_size );
    virtual void train_SGD(const uint64_t &epoch , const bool &endCondition , const uint64_t &batch_size);

    void train_MOMENTUM(const uint64_t &epoch , const uint64_t &batch_size  , const scalar &beta);
    virtual void train_MOMENTUM(const uint64_t &epoch , const bool &endCondition , const uint64_t &batch_size  , const scalar &beta);

    void train_RMS(const uint64_t &epoch , const uint64_t &batch_size , const scalar &beta , const scalar &epsilon);
    virtual void train_RMS(const uint64_t &epoch , const bool &endCondition , const uint64_t &batch_size , const scalar &beta , const scalar &epsilon);

    void train_ADAM(const uint64_t &epoch , const uint64_t &batch_size , const scalar &beta1  = 0.9f, const scalar &beta2  = 0.999f, const scalar &epsilon = 10e-8f);
    virtual void train_ADAM(const uint64_t &epoch , const bool &endCondition , const uint64_t &batch_size , const scalar &beta1 = 0.9f , const scalar &beta2  =0.999f, const scalar &epsilon = 10e-8f);

    virtual scalar cost(const ste::Matrix<scalar> &expected_output , const ste::Matrix<scalar> &obtained_output) const = 0;                     //Computes the cost function
    scalar loss(const ste::Matrix<scalar> &expected_output , const ste::Matrix<scalar> &obtained_output) const;
#endif





};//class NeuralNetwork

}//namespace MNN



#endif // NEURALNETWORK_H
