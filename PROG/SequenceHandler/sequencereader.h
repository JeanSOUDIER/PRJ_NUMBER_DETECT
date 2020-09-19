#ifndef SEQUENCEREADER_H
#define SEQUENCEREADER_H

#include "SequenceHandler_global.h"

#include "sequence.h"



#include <QFile>
#include <QFileInfo>

#include <QString>

#include <QTextStream>

#include <QXmlStreamReader>


class SequenceReader{

    friend class SequenceHandler;

public:

    SequenceReader(const QString &path);

protected:

    bool isValid(const QString &path);
    Sequence readSequence(const QString &path);
    Sequence readSequence();

private :

    bool isValid_XML(const QString &path);
    bool isValid_CSV(const QString &path);

    Sequence readSequence_XML(const QString &path);
    Sequence readSequence_CSV(const QString &path);

    Movement* constructMovement_CSV(const QVector<QString> &);



    QString path;
};

#endif // SEQUENCEREADER_H
