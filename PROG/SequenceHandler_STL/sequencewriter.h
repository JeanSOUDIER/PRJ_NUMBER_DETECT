#ifndef SEQUENCEWRITER_H
#define SEQUENCEWRITER_H

#include "SequenceHandler_STL_global.h"
#include "movement.h"
#include "utility.h"

#include <string>
#include <vector>


class SequenceWriter{


public:

    SequenceWriter(const std::vector<Movement> &_movements, const std::string _filePath);


    void setFile(const std::string &path);
    static void remove(const std::string &path);


protected:

    bool generate(const Format , const bool withOverride);
    bool generate_XML(const bool withOverride);
    bool generate_CSV(const bool withOverride);

private:

    std::vector<Movement> movements;
    std::string filePath;


};

#endif // SEQUENCEWRITER_H
