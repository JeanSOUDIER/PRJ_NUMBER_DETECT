#ifndef SEQUENCEWRITER_H
#define SEQUENCEWRITER_H

#include "movement.h"

#include <QVector>

#include <QFile>
#include <QFileInfo>

#include <QString>

#include <QTextStream>

#include <QXmlStreamReader>

class MainWindow;

class SequenceWriter{

    friend class MainWindow;

public:

    SequenceWriter(const QVector<Movement*> _movements, const QString _filePath);

protected:

    bool generate(const Format);
    bool generate_XML();
    bool generate_CSV();

private:

    QVector<Movement*> movements;
    QString filePath;


};

#endif // SEQUENCEWRITER_H
