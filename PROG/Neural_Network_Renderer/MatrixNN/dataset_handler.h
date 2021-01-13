#ifndef DATASET_HANDLER_H
#define DATASET_HANDLER_H


#include "Matrix/Matrix.hpp"
#include "MatrixNN_global.h"

#include <string>
#include <vector>

namespace MNN {


class Dataset_Handler {

public:


    enum class SET{
        TRAIN,
        TRAINING = TRAIN,

        TEST
    };



    Dataset_Handler(const std::string &source, const SET &type);
    virtual ~Dataset_Handler();



#ifndef MNN_LOW_RAM
    virtual const ste::Matrix<MNN::scalar>& data(const unsigned long long &index) const = 0;
    virtual const long long int& label(const unsigned long long &index) const = 0;
    virtual void load(const std::string &path) = 0;
    virtual void load() =0;
#else
    virtual const ste::Matrix<scalar> data(const unsigned long long &index) const = 0;
    virtual long long int label(const unsigned long long &index) const = 0;
#endif

    SET type() const;


    uint64_t size() const;


protected:

    std::string _source;
    std::string _source_delimiter;
    SET _type;

#ifndef MNN_LOW_RAM
    std::vector<ste::Matrix<MNN::scalar>> _data;
    std::vector<long long int> _labels;
#endif





};

} //namespace MNN




#endif // DATASET_HANDLER_H
