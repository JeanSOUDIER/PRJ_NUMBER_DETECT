#include "sequence.h"

//#include <QDebug>

Sequence::Sequence(){}
Sequence::~Sequence(){qDeleteAll(movements);}


/**************************************************************/

void Sequence::addMovement(Movement *mov){movements.append(mov);}
void Sequence::removeMovement(const unsigned index){movements.remove(index);}
void Sequence::replaceMovement(const unsigned index, Movement* mov){movements.replace(index, mov);}
void Sequence::clear(bool withDelete){

    if(withDelete){qDeleteAll(movements);}
    movements.clear();

}

/**************************************************************/

QVector<Movement*> Sequence::getMovements(){return movements;}
std::vector<Movement*> Sequence::getMovements_STD(){return movements.toStdVector();}

QVector<Movement*> Sequence::getMovements() const{return movements;}
std::vector<Movement*> Sequence::getMovements_STD() const{return movements.toStdVector();}

void Sequence::setMovements(const QVector<Movement*>&_movements){movements = _movements;}
void Sequence::setMovements(const std::vector<Movement*>&_movements){movements = QVector<Movement*>::fromStdVector(_movements);}


/**************************************************************/


bool Sequence::operator ==(const Sequence& seq) const{return equals(*this , seq);}
bool Sequence::equals (const Sequence &seq1 , const Sequence &seq2){return std::addressof(seq1) == std::addressof(seq2);}
bool Sequence::isEquivalent(const Sequence &seq) const{

    if(seq.movements.size() != movements.size()){return false;}

    for(int index = 0 ; index < movements.size() ; index++){
        if(!movements.at(index)->isEquivalent(*seq.movements.at(index))){return false;}
    }


    return true;
}


