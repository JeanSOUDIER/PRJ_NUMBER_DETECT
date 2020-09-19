#ifndef SEQUENCEHANDLER_H
#define SEQUENCEHANDLER_H

#include "SequenceHandler_global.h"
#include "sequence.h"
#include "sequencereader.h"


#include <QString>

#include <QVector>
#include <vector>

class SequenceHandler{

public:

    SequenceHandler(const unsigned char &character , const QString &path);
    SequenceHandler();

    const Sequence find(const unsigned char&);
    unsigned char find(const Sequence&);

    void addSequence(const unsigned char &character , const QString &path);


private:

    QVector<Sequence> sequences;
    QVector<unsigned char> characters;

};

#endif // SEQUENCEHANDLER_H
