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
    Sequence(QVector<Movement> mov);
    Sequence(std::vector<Movement> mov);

    QVector<Movement> getMovements();
    std::vector<Movement> getMovements_STD();
    QVector<Movement> getMovements() const;
    std::vector<Movement> getMovements_STD() const;

    void setMovements(const QVector<Movement>&);
    void setMovements(const std::vector<Movement>&);

    static QVector<Movement> generateSegment(const Movement &begin , const Movement &end , const uint64_t step_x , const uint64_t step_y , const uint64_t step_z, const uint64_t delay_ms);
    static std::vector<Movement> generateSegment_STD(const Movement &begin , const Movement &end , const uint64_t step_x , const uint64_t step_y , const uint64_t step_z, const uint64_t delay_ms);

    static QVector<Movement> generateSegment(const Movement &begin , const Movement &end , const uint64_t step, const uint64_t delay_ms);
    static std::vector<Movement> generateSegment_STD(const Movement &begin , const Movement &end , const uint64_t step , const uint64_t delay_ms);


    static QVector<Movement> generateSegment(const Movement &begin , const Movement &end , const uint64_t step);
    static std::vector<Movement> generateSegment_STD(const Movement &begin , const Movement &end , const uint64_t step);

    static QVector<Movement> generateSegment(const Movement &begin , const Movement &end);
    static std::vector<Movement> generateSegment_STD(const Movement &begin , const Movement &end);

    bool operator ==(const Sequence& seq) const;
    static bool equals (const Sequence &seq1 , const Sequence &seq2);
    bool isEquivalent(const Sequence &seq) const;

protected:

    void addMovement(Movement);
    void removeMovement(const unsigned index);
    void replaceMovement(const unsigned index, Movement);
    void clear();


private:

    QVector<Movement> movements;

};

#endif // SEQUENCE_H
