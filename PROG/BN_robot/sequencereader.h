#ifndef SEQUENCEREADER_H
#define SEQUENCEREADER_H

#include "SequenceHandler_STL_global.h"

#include "sequence.h"
#include "utility.h"

#include <string>

class SequenceReader{

    friend class SequenceHandler;

public:

    SequenceReader(const std::string &path);
    Sequence readSequence();
protected:

    bool isValid(const std::string &path);
    Sequence readSequence(const std::string &path);


private :

    bool isValid_XML(const std::string &path);
    bool isValid_CSV(const std::string &path);

    Sequence readSequence_XML(const std::string &path);
    Sequence readSequence_CSV(const std::string &path);

    Movement constructMovement_CSV(const std::vector<std::string> &);



    std::string path;
};

#endif // SEQUENCEREADER_H
