#include "sequencehandler.h"

SequenceHandler::SequenceHandler(const unsigned char &character, const std::string &path){

    SequenceReader reader(path);

    characters.push_back(character);
    sequences.push_back(reader.readSequence());

}


SequenceHandler::SequenceHandler(){}

/**************************************************************/

const Sequence SequenceHandler::find(const unsigned char& character){

    const std::ptrdiff_t index = (std::find(characters.begin(), characters.end(), character) != characters.end()) ? std::distance(characters.begin(), std::find(characters.begin(), characters.end(), character)) : -1;
    return (index > -1) ? sequences.at(index): Sequence();


}
unsigned char SequenceHandler::find(const Sequence& seq){

    const std::ptrdiff_t index = (std::find(sequences.begin(), sequences.end(), seq) != sequences.end()) ? std::distance(sequences.begin(), std::find(sequences.begin(), sequences.end(), seq)) : -1;
    return (index > -1) ? characters.at(index): '\0';
}

void SequenceHandler::addSequence(const unsigned char &character , const std::string &path){

        if(std::find(characters.begin(), characters.end(), character) != characters.end()){
            std::cout << "nope : " << character <<"\n";
            return;
        }



    SequenceReader reader(path);

    characters.push_back(character);
    sequences.push_back(reader.readSequence());

}
