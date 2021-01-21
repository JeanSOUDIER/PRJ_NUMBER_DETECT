#include "dataset_handler.h"

namespace MNN {


Dataset_Handler::Dataset_Handler(const std::string &source, const SET &type){

    _source = source;
    _type = type;

}


Dataset_Handler::~Dataset_Handler(){}

Dataset_Handler::SET Dataset_Handler::type() const{return _type;}

uint64_t Dataset_Handler::size() const{return _labels.size();}

} //namespace MNN

