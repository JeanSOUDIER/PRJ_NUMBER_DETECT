#ifndef MNIST_HANDLER_H
#define MNIST_HANDLER_H


#include "../SequenceHandler_STL/utility.h"

#include "../MatrixNN_global.h"

#include "../dataset_handler.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <stdexcept>

#include <vector>


class MNIST_Handler : public MNN::Dataset_Handler {

public:

    MNIST_Handler(const std::string &source_csv , const std::string &source_delimiter, const MNN::Dataset_Handler::SET &type);
    virtual ~MNIST_Handler();



#ifndef MNN_LOW_RAM
    virtual const ste::Matrix<MNN::scalar>& data(const unsigned long long &index) const override;
    virtual const long long int& label(const unsigned long long &index) const override;
    virtual void load(const std::string &path) override;
    virtual void load(const std::string &path, const std::string &delimiter);
    virtual void load() override;
#else
    virtual const ste::Matrix<MNN::scalar> data(const unsigned long long &index) const override;
    virtual long long int label(const unsigned long long &index) const override;
#endif

protected:

    std::string _source_delimiter;

private:





};

#endif // MNIST_HANDLER_H
