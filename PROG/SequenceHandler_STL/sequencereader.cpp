#include "sequencereader.h"



SequenceReader::SequenceReader(const std::string &_path){path = _path;}

/**************************************************************/

bool SequenceReader::isValid(const std::string &path){return (isValid_XML(path) || isValid_CSV(path));}


bool SequenceReader::isValid_XML(const std::string &path){
    (void)path;
    return false;
}



bool SequenceReader::isValid_CSV(const std::string &path){

    bool success = true;

    std::ifstream file(path);

    if(file.is_open()){

         std::string line;

         while(std::getline(file , line) && success){
             const unsigned int data_size = Utility::split(line , ",").size();
             success = (data_size == 2 || data_size == 4);
         }

    }
    else{success = false;}

    file.close();

    return success;
}




Sequence SequenceReader::readSequence(const std::string &path){

    if(isValid_XML(path)){return readSequence_XML(path);}
    if(isValid_CSV(path)){return readSequence_CSV(path);}

    return Sequence();


}

Sequence SequenceReader::readSequence(){return readSequence(path);}

Sequence SequenceReader::readSequence_XML(const std::string &path){
    (void)path;
    return Sequence();
}

Sequence SequenceReader::readSequence_CSV(const std::string &path){

    Sequence seq;

    std::ifstream file(path);

    if(file.is_open()){

         std::string line;

         while(std::getline(file , line)){
             seq.addMovement(constructMovement_CSV(Utility::split(line , ",")));
         }

    }

    file.close();

    return seq;
}

Movement SequenceReader::constructMovement_CSV(const std::vector<std::string> &data ){

    Movement mov;
    switch(data.size()){

        case(2):{
            mov.setMode(MovementMode::DIRECTION , false);
            mov.setDirection(static_cast<Direction>(std::stoi(data.at(0))));
            mov.setDuration(static_cast<uint64_t>(std::stoi(data.at(1))));
            break;
        }

        case(4):{
            //std::cout << "mov_coor";;
            mov.setMode(MovementMode::COORDINATES , false);
            mov.setX(std::stoi(data.at(0)));
            mov.setY(std::stoi(data.at(1)));
            mov.setZ(std::stoi(data.at(2)));
            mov.setDuration(static_cast<uint64_t>(std::stoi(data.at(3))));
            /*std::cout << mov->getX();
            std::cout << mov->getY();
            std::cout << mov->getZ();
            std::cout << mov->getDuration();*/
            break;
        }

        default:{break;}

    }

    return mov;


}

