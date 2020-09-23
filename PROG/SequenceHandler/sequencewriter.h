#ifndef SEQUENCEWRITER_H
#define SEQUENCEWRITER_H

#include "SequenceHandler_global.h"
#include "movement.h"

#include <QVector>

#include <QFile>
#include <QFileInfo>

#include <QString>

#include <QTextStream>

#include <QXmlStreamReader>


class SequenceWriter{


public:

    SequenceWriter(const QVector<Movement*> &_movements, const QString _filePath);
    SequenceWriter(const std::vector<Movement *> &_movements, const std::string _filePath);


    void setFile(const QString &path);
    static void remove(const QString &path);


protected:

    bool generate(const Format , const bool withOverride);
    bool generate_XML(const bool withOverride);
    bool generate_CSV(const bool withOverride);

private:

    QVector<Movement*> movements;
    QString filePath;


};

#endif // SEQUENCEWRITER_H
