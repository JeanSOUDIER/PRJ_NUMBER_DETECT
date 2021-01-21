#ifndef PROJECT_NETWORK_H
#define PROJECT_NETWORK_H

#include "../neuralnetwork.h"

#ifdef MNN_TRAIN
#include "../MNIST_Handler/mnist_handler.h"
#endif

class Project_network final : public MNN::NeuralNetwork {

public:
    Project_network(const uint64_t &layer_number = 0);

    #ifdef MNN_TRAIN
    Project_network(const MNN::scalar &learning_rate , const uint64_t &layer_number ,
                    const std::string &save_path = "",
                    const std::string &path_src_TRAIN = "", const std::string &delimiter_TRAIN = "," ,
                    const std::string &path_src_VALIDATION = "" , const std::string &delimiter_VALIDATION = ",");
    #endif

    ~Project_network();

#ifdef MNN_TRAIN
    ste::Matrix<MNN::scalar> expected(const long long int &label) const override;
    uint64_t test(const uint64_t &size);
#endif

protected:

#ifdef MNN_TRAIN
//    void train_SGD(const uint64_t &epoch , const bool &endCondition , const uint64_t &batch_size) override;
//    void train_MOMENTUM(const uint64_t &epoch , const bool &endCondition , const uint64_t &batch_size  , const MNN::scalar &beta) override;
//    void train_RMS(const uint64_t &epoch , const bool &endCondition , const uint64_t &batch_size , const MNN::scalar &beta , const MNN::scalar &epsilon) override;
//    void train_ADAM(const uint64_t &epoch , const bool &endCondition , const uint64_t &batch_size , const MNN::scalar &beta1 = 0.9f , const MNN::scalar &beta2  =0.999f, const MNN::scalar &epsilon = 10e-8f) override;

    MNN::scalar cost(const ste::Matrix<MNN::scalar> &expected_output , const ste::Matrix<MNN::scalar> &obtained_output) const override;
#endif






};

#endif // PROJECT_NETWORK_H
