#include "mnist_handler.h"

MNIST_Handler::MNIST_Handler(const std::string &source_csv, const std::string &source_delimiter, const SET &type) : MNN::Dataset_Handler(source_csv , type){

    _source_delimiter = source_delimiter;

#ifndef MNN_LOW_RAM
    load();
#endif

}

MNIST_Handler::~MNIST_Handler(){}

#ifndef MNN_LOW_RAM

const ste::Matrix<MNN::scalar> &MNIST_Handler::data(const unsigned long long &index) const{
    if(index >= _labels.size()){throw std::invalid_argument("MNIST_Handler::number_data\nIndex is outside range. Received"
                                                               + std::to_string(index)
                                                               + " but maximum is " + std::to_string(_labels.size()-1) + ".");}
    return _data.at(index);
}


const long long int& MNIST_Handler::label(const unsigned long long &index) const{


    if(index >= _labels.size()){throw std::invalid_argument("MNIST_Handler::label\nIndex is outside range. Received "
                                                               + std::to_string(index)
                                                               + " but maximum is " + std::to_string(_labels.size()-1) + ".");}
    return _labels.at(index);
}


void MNIST_Handler::load(const std::string &path , const std::string &delimiter){

    if(_labels.size() > 0){return;} //Do not load twice

    _data.reserve((_type == SET::TRAIN) ? 60000 : 10000);

    std::cout << "Loading MNIST "<< ((_type == SET::TRAIN) ? "training" : "validation") << " data set..." << std::endl;

#ifdef MNN_DOUBLE_PRECISION
    const std::vector<std::vector<MNN::scalar>> data  = Utility::readCSV_DOUBLE(path , delimiter);
#else
    const std::vector<std::vector<MNN::scalar>> data = Utility::readCSV_FLOAT(path , delimiter);
#endif

    for(const std::vector<MNN::scalar> &line : data){

        _data.push_back(ste::Matrix<MNN::scalar>(std::vector<MNN::scalar>(line.begin() +1 , line.end()) ,
                                                 line.size()-1 , 1));

        _data.back().for_each([](MNN::scalar &value){value /= 255.;});
        _labels.push_back(line.at(0));
    }

    std::cout << "Loading complete." << std::endl;

}

void MNIST_Handler::load(const std::string &path){load(path , _source_delimiter);}

void MNIST_Handler::load(){load(_source , _source_delimiter);}


#else


const ste::Matrix<MNN::scalar> MNIST_Handler::data(const unsigned long long &index) const {

    if(Utility::fileSize(_source) <=0){throw std::runtime_error("MNIST_Handler::number_data\nCannot find source file.");}

    std::ifstream file(_source);
    if(file.is_open()){

        if(index > 0){
            file.seekg(std::ios::beg);
            for(uint64_t i=0; i < index; ++i){
                file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            }
        }
        std::string line;
        std::getline(file , line);

        if(line.empty()){throw std::runtime_error("MNIST_Handler::number_data\nRetrived empty data from line. Please make sure the index this valid.");}

        std::vector<std::string> line_data = Utility::split(line , _source_delimiter);

        std::vector<MNN::scalar> result;
        result.reserve(line_data.size()-1);

        std::transform(line_data.begin()+1, line_data.end() , std::back_inserter(result ), [](std::string str) {return std::stof(str)/255.;});

        file.close();
                return ste::Matrix<MNN::scalar>(result , result.size() , 1);;

    }

    file.close();
    return ste::Matrix<MNN::scalar>();


}

long long int MNIST_Handler::label(const unsigned long long &index) const {


    if(Utility::fileSize(_source) <=0){throw std::runtime_error("MNIST_Handler::label\nCannot find source file.");}

    std::ifstream file(_source);
    if(file.is_open()){

        if(index > 0){
            file.seekg(std::ios::beg);
            for(uint64_t i=0; i < index; ++i){
                file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            }
        }

         std::string line;
         std::getline(file , line);



         if(line.empty()){throw std::runtime_error("MNIST_Handler::number_data\nRetrived empty data from line. Please make sure the index this valid.");}


          file.close();
         return std::stoi(std::string(1 , line[0]));
    }

    file.close();
    return 0;

}






#endif



