#include "sequencehandler.h"

SequenceHandler::SequenceHandler(const unsigned char &character, const QString &path){

    SequenceReader reader(path);

    characters.append(character);
    sequences.append(reader.readSequence());

}


SequenceHandler::SequenceHandler(){}

/**************************************************************/

const Sequence SequenceHandler::find(const unsigned char& character){

    const int index = characters.indexOf(character);
    return (index > -1) ? sequences.at(index): Sequence();


}
unsigned char SequenceHandler::find(const Sequence& seq){

    const int index = sequences.indexOf(seq);
    return (index > -1) ? characters.at(index): '0';
}

void SequenceHandler::addSequence(const unsigned char&character , const QString &path){

    SequenceReader reader(path);

    if(characters.contains(character)){return;}

    characters.append(character);
    sequences.append(reader.readSequence());

}
