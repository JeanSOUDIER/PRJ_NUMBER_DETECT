#include "sequencewriter.h"

SequenceWriter::SequenceWriter(const std::vector<Movement> &_movements, const std::string _filePath){

    movements = _movements;
    filePath = _filePath;

}


/**************************************************************/

void SequenceWriter::remove(const std::string &path){

    if(Utility::exists(path)){std::remove(&path[0]);}

}

/**************************************************************/

bool SequenceWriter::generate(const Format format, const bool withOverride){

    if(movements.empty()){return false;}

    switch(format){

        case(Format::XML):{return (Utility::exists(filePath + ".xml") && !withOverride) ? false  : generate_XML(withOverride);}
        case(Format::CSV):{return (Utility::exists(filePath + ".csv") && !withOverride) ? false  : generate_CSV(withOverride);}
        case(Format::ALL):{return ((Utility::exists(filePath + ".xml") || Utility::exists(filePath + ".csv") ) && !withOverride) ? false  : generate_XML(withOverride) && generate_CSV(withOverride);}
        default:{return false;}

    }

}

/**************************************************************/

bool SequenceWriter::generate_XML(const bool withOverride){

    (void)withOverride;
    return false;

}

/**************************************************************/

bool SequenceWriter::generate_CSV(const bool withOverride){

    if(withOverride){remove(filePath + ".csv");}

     std::ofstream file(filePath + ".csv");

     const MovementMode movements_mode_format = movements.at(0).getMode();

     if(file.is_open()){

         for(unsigned int index = 0 ; index < movements.size() ; index++){

             switch(movements_mode_format){

                 case(MovementMode::DIRECTION):{
                 file<< static_cast<int>(movements.at(index).getDirection()) <<",";
                 break;
                 }
                 case(MovementMode::COORDINATES):{
                 file << movements.at(index).getX() << "," << movements.at(index).getY() << "," << movements.at(index).getZ() <<",";
                 break;
                 }
                 default:{break;}

             }
             file << movements.at(index).getDuration() << std::endl;
         }

     }

     else{return false;}

     file.close();


 return true;


}
/**************************************************************/

void SequenceWriter::setFile(const std::string &path){filePath = path;}
