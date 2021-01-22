#ifndef MNIST_HANDLER_H
#define MNIST_HANDLER_H


#include "../SequenceHandler_STL/utility.h"

#include "../MatrixNN_global.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <stdexcept>

#include <vector>


class MNIST_Handler {



public:

    enum class SET{
        TRAIN,
        TRAINING = TRAIN,

        VALIDATION
    };

    enum class LOADING{
        READ_ALL,
        READ_ONE_LINE
    };

    MNIST_Handler(const std::string &source_csv , const std::string &source_delimiter, const SET &type , const LOADING &loading);
    virtual ~MNIST_Handler();


    virtual std::vector<std::vector<MNN::scalar>> raw_data() const;

    virtual int label(unsigned long long index) const; //if(index > raw_data().size()){throw std::invalid_argument("MNIST_Handler::label\nIndex is outside range. Expected");}

    virtual const ste::Matrix<MNN::scalar> number_data(unsigned long long index) const; //if(index > raw_data().size()){throw std::invalid_argument("MNIST_Handler::label\nIndex is outside range. Expected");}

    SET type() const;

    virtual void load(const std::string &path, const std::string &delimiter);
    virtual void load();


protected:


private:

    std::string _source_csv;
    std::string _source_delimiter;

    std::vector<std::vector<MNN::scalar>> _raw_data;
    SET _type;
    LOADING _loading_type;


};

#endif // MNIST_HANDLER_H
