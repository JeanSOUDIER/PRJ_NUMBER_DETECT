#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "movement.h"

#include <QVector>

class SequenceBox;
class c;
class SequenceWriter;
class MovementBox;
class MainWindow;

class Sequence{


    friend class SequenceBox;
    friend class SequenceReader;
    friend class SequenceWriter;
    friend class MainWindow;
    friend class MovementBox;

public:

    Sequence();
    ~Sequence();

    QVector<Movement*> getMovements();
    void setMovements(const QVector<Movement*>);

protected:

    void addMovement(Movement *);
    void removeMovement(const unsigned index);
    void replaceMovement(const unsigned index, Movement *);
    void clear(bool withDelete);


private:

    QVector<Movement*> movements;

};

#endif // SEQUENCE_H
