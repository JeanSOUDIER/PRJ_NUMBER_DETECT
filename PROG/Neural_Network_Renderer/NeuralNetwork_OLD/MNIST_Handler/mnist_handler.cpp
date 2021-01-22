#include "mnist_handler.h"

MNIST_Handler::MNIST_Handler(const std::string &source_csv, const std::string &source_delimiter, const SET &type, const LOADING &loading){

    _source_csv = source_csv;
    _source_delimiter = source_delimiter;

    _type = type;
    _loading_type = loading;

    switch(_loading_type){

        case(LOADING::READ_ALL):{load(); break;}
        case(LOADING::READ_ONE_LINE):{break;}
        default:{throw std::invalid_argument("MNIST_Handler::MNIST_Handler\nInvalid loading mode."); break;}

    }

}

MNIST_Handler::~MNIST_Handler(){}


int MNIST_Handler::label(unsigned long long index) const{

    switch(_loading_type){

        case(LOADING::READ_ALL):{
            if(index >= raw_data().size()){throw std::invalid_argument("MNIST_Handler::label\nIndex is outside range. Received "
                                                                       + std::to_string(index)
                                                                       + " but maximum is " + std::to_string(raw_data().size()-1) + ".");}

            return _raw_data.at(index).at(0);

        break;
        }


        case(LOADING::READ_ONE_LINE):{
        if(Utility::fileSize(_source_csv) <=0){throw std::runtime_error("MNIST_Handler::label\nCannot find source file.");}

        std::ifstream file(_source_csv);
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

        }else{throw std::runtime_error("Cannot open source file.\nSpecified path was " + _source_csv + " .");}



        break;
        }


        default:{throw std::invalid_argument("MNIST_Handler::label\nInvalid loading mode."); break;}


    }

    return '0';

}

const ste::Matrix<MNN::scalar> MNIST_Handler::number_data(unsigned long long index) const{

    switch(_loading_type){

        case(LOADING::READ_ALL):{
            if(index >= raw_data().size()){throw std::invalid_argument("MNIST_Handler::number_data\nIndex is outside range. Received"
                                                                       + std::to_string(index)
                                                                       + " but maximum is " + std::to_string(raw_data().size()-1) + ".");}

            const std::vector<MNN::scalar> data(_raw_data.at(index).begin() +1 , _raw_data.at(index).end());

            return ste::Matrix<MNN::scalar>(data , data.size() , 1);

        break;
        }


        case(LOADING::READ_ONE_LINE):{

            if(Utility::fileSize(_source_csv) <=0){throw std::runtime_error("MNIST_Handler::number_data\nCannot find source file.");}

            std::ifstream file(_source_csv);
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


                        return ste::Matrix<MNN::scalar>(result , result.size() , 1);;


            file.close();

            }else{throw std::runtime_error("MNIST_Handler::number_data\nCannot open source file.\nSpecified path was " + _source_csv + " .");}


            break;
        }


        default:{throw std::invalid_argument("MNIST_Handler::number_data\nInvalid loading mode."); break;}


    }

    return ste::Matrix<MNN::scalar>(0);

}


void MNIST_Handler::load(const std::string &path , const std::string &delimiter){

    if(_loading_type != LOADING::READ_ALL){return;}
    if(_raw_data.size() > 0){return;} //Do not load twice

    switch(_type){

        case(SET::TRAIN):{
            _raw_data.reserve(60000);
        break;
        }
        case(SET::VALIDATION):{
            _raw_data.reserve(10000);
        break;
        }
        default:{throw std::invalid_argument("MNIST_Handler::MNIST_Handler\nInvalid loading mode."); break;}

    }

    std::cout << "Loading MNIST "<< ((_type == SET::TRAIN) ? "training" : "validation") << " data set..." << std::endl;

#ifdef DOUBLE_PRECISION
    _raw_data = Utility::readCSV_DOUBLE(path , delimiter);
#else
    _raw_data = Utility::readCSV_FLOAT(path , delimiter);
#endif

    for(auto &line : _raw_data){

        //for(auto &value : line){value /= 255.;}
        for(uint64_t item = 1 ; item < line.size() ; item++){line.at(item) /= 255.;}
    }

    std::cout << "Loading complete." << std::endl;

}
void MNIST_Handler::load(){load(_source_csv , _source_delimiter);}

std::vector<std::vector<MNN::scalar> > MNIST_Handler::raw_data() const{return _raw_data;}
MNIST_Handler::SET MNIST_Handler::type() const{return _type;}
