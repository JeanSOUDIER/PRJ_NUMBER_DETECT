#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "movement.h"

#include <memory> //std::addressof

#include <QVector>

class Sequence{


    friend class SequenceReader;
    friend class SequenceHandler;

public:

    Sequence();
    ~Sequence();

    QVector<Movement*> getMovements();
    std::vector<Movement*> getMovements_STD();
    QVector<Movement*> getMovements() const;
    std::vector<Movement*> getMovements_STD() const;

    void setMovements(const QVector<Movement*>&);
    void setMovements(const std::vector<Movement*>&);

    bool operator ==(const Sequence& seq) const;
    static bool equals (const Sequence &seq1 , const Sequence &seq2);
    bool isEquivalent(const Sequence &seq) const;

protected:

    void addMovement(Movement *);
    void removeMovement(const unsigned index);
    void replaceMovement(const unsigned index, Movement *);
    void clear(bool withDelete);


private:

    QVector<Movement*> movements;

};

#endif // SEQUENCE_H
