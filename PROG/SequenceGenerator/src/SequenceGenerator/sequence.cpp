#include "sequence.h"

//#include <QDebug>

Sequence::Sequence(){}
Sequence::~Sequence(){qDeleteAll(movements);}

/**************************************************************/

QVector<Movement*> Sequence::getMovements(){return movements;}
void Sequence::setMovements(const QVector<Movement*>_movements){movements = _movements;}

/**************************************************************/

void Sequence::addMovement(Movement *mov){movements.append(mov);}
void Sequence::removeMovement(const unsigned index){movements.remove(index);}
void Sequence::replaceMovement(const unsigned index, Movement* mov){movements.replace(index, mov);}
void Sequence::clear(bool withDelete){

    if(withDelete){qDeleteAll(movements);}
    movements.clear();

}
