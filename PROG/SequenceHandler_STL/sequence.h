#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "movement.h"


#include <memory> //std::addressof
#include <vector>
#include <string>


class Sequence{


    friend class SequenceReader;
    friend class SequenceHandler;

public:

    Sequence();
    Sequence(std::vector<Movement> mov);

    std::vector<Movement> getMovements();
    std::vector<Movement> getMovements() const;

    void setMovements(const std::vector<Movement>&);

    static std::vector<Movement> generateSegment(const Movement &begin , const Movement &end , const uint64_t step_x , const uint64_t step_y , const uint64_t step_z, const uint64_t delay_ms);

    static std::vector<Movement> generateSegment(const Movement &begin , const Movement &end , const uint64_t step, const uint64_t delay_ms);


    static std::vector<Movement> generateSegment(const Movement &begin , const Movement &end , const uint64_t step);

    static std::vector<Movement> generateSegment(const Movement &begin , const Movement &end);

    bool operator ==(const Sequence& seq) const;
    static bool equals (const Sequence &seq1 , const Sequence &seq2);
    bool isEquivalent(const Sequence &seq) const;

protected:

    void addMovement(Movement);
    void removeMovement(const unsigned index);
    void replaceMovement(const unsigned index, Movement);
    void clear();


private:

    std::vector<Movement> movements;

};

#endif // SEQUENCE_H
