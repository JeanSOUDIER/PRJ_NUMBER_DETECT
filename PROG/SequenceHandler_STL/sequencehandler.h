#ifndef SEQUENCEHANDLER_H
#define SEQUENCEHANDLER_H

#include "SequenceHandler_STL_global.h"
#include "sequence.h"
#include "sequencereader.h"


#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>


class SequenceHandler{

public:

    SequenceHandler(const unsigned char &character , const std::string &path);
    SequenceHandler();

    const Sequence find(const unsigned char&);
    unsigned char find(const Sequence&);

    void addSequence(const unsigned char &character , const std::string &path);


private:

    std::vector<Sequence> sequences;
    std::vector<unsigned char> characters;

};

#endif // SEQUENCEHANDLER_H
