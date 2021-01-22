#ifndef PROJECT_NETWORK_H
#define PROJECT_NETWORK_H

#include "../neuralnetwork.h"

#include "../MNIST_Handler/mnist_handler.h"

class Project_network final : public MNN::NeuralNetwork {

public:
    Project_network();
    #ifdef MNN_TRAIN
    Project_network(const MNN::scalar &learning_rate , const std::vector<uint64_t> &layers_cells ,
                    const std::string &save_path = "",
                    const std::string &path_src_TRAIN = "", const std::string &delimiter_TRAIN = "," ,
                    const std::string &path_src_VALIDATION = "" , const std::string &delimiter_VALIDATION = ",");
    #endif
    ~Project_network();

    long long int predict(const ste::Matrix<MNN::scalar> &input) const override;

#ifdef MNN_TRAIN
    ste::Matrix<MNN::scalar> expected(const long long int &label) const override;
    uint64_t validate(const uint64_t &size);
#endif

protected:

#ifdef MNN_TRAIN
    void train_SGD(const uint64_t &epoch , const bool &endCondition , const uint64_t &batch_size) override;
    void train_MOMENTUM(const uint64_t &epoch , const bool &endCondition , const uint64_t &batch_size  , const MNN::scalar &beta) override;
    void train_RMS(const uint64_t &epoch , const bool &endCondition , const uint64_t &batch_size , const MNN::scalar &beta , const MNN::scalar &epsilon) override;
    void train_ADAM(const uint64_t &epoch , const bool &endCondition , const uint64_t &batch_size , const MNN::scalar &beta1 = 0.9f , const MNN::scalar &beta2  =0.999f, const MNN::scalar &epsilon = 10e-8f) override;
#endif

   MNN::scalar cost(const ste::Matrix<MNN::scalar> &expected_output , const ste::Matrix<MNN::scalar> &obtained_output) const override;


private:

#ifdef MNN_TRAIN
    MNIST_Handler *mnist_training;
    MNIST_Handler *mnist_validation;
#endif
};

#endif // PROJECT_NETWORK_H
