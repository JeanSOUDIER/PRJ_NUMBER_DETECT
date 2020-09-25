#include "sequence.h"

Sequence::Sequence(){}

Sequence::Sequence(std::vector<Movement> mov){movements = mov;}


/**************************************************************/

void Sequence::addMovement(Movement mov){movements.push_back(Movement(mov));}
void Sequence::removeMovement(const unsigned index){movements.erase(movements.begin() + index);}
void Sequence::replaceMovement(const unsigned index, Movement mov){movements.at(index) = Movement(mov);}
void Sequence::clear(){movements.clear();}

/**************************************************************/

std::vector<Movement> Sequence::getMovements(){return movements;}
std::vector<Movement> Sequence::getMovements() const{return movements;}

void Sequence::setMovements(const std::vector<Movement>&_movements){movements = _movements;}

/**************************************************************/

std::vector<Movement> Sequence::generateSegment(const Movement &begin , const Movement &end , const uint64_t step_x, const uint64_t step_y, const uint64_t step_z , const uint64_t delay_ms){

    if(begin.positionMatches(end)){return std::vector<Movement>{begin , end};}

    Movement currentMovement = begin;
    std::vector<Movement> output = {Movement(currentMovement)};

    const long int min_x = std::min(begin.getX() , end.getX());
    const long int min_y = std::min(begin.getY() , end.getY());
    const long int min_z = std::min(begin.getZ() , end.getZ());

    const long int max_x = std::max(begin.getX() , end.getX());
    const long int max_y = std::max(begin.getY() , end.getY());
    const long int max_z = std::max(begin.getZ() , end.getZ());

    const bool x_is_inverted = !(begin.getX() == min_x);
    const bool y_is_inverted = !(begin.getY() == min_y);
    const bool z_is_inverted = !(begin.getZ() == min_z);
   /* qDebug() << "x_is_inveted: " << x_is_inverted;
    qDebug() << "y_is_inveted: " << y_is_inverted;
    qDebug() << "z_is_inveted: " << z_is_inverted;*/

    const long long step_x_corrected = (x_is_inverted ? -step_x : step_x);
    const long long step_y_corrected = (y_is_inverted ? -step_y : step_y);
    const long long step_z_corrected = (z_is_inverted ? -step_z : step_z);
    /*qDebug() << "step_x : " << step_x_corrected;
    qDebug() << "step_y : " << step_y_corrected;
    qDebug() << "step_z : " << step_z_corrected << "\n\n";*/

    bool ended = false;

    //unsigned counter = 0;

    while(!ended){

        const bool x_condition = (x_is_inverted ? (currentMovement.getX() /*+ step_x_corrected*/ <= min_x) :(currentMovement.getX() /*+ step_x_corrected*/ >= max_x));
        const bool y_condition = (y_is_inverted ? (currentMovement.getY()/* + step_y_corrected */<= min_y) :(currentMovement.getY()/* + step_y_corrected */>= max_y));
        const bool z_condition = (z_is_inverted ? (currentMovement.getZ() /*+ step_z_corrected */<= min_z) :(currentMovement.getZ() /*+ step_z_corrected */>= max_z));
        ended = x_condition && y_condition && z_condition;
        /*counter++;
        qDebug() << "Iteration: " << counter;
        qDebug() << "x_condition: " << x_condition;
        qDebug() << "y_condition: " << y_condition;
        qDebug() << "z_condition: " << z_condition << "\n\n";*/

        if(!x_condition){currentMovement.setX(currentMovement.getX() + step_x_corrected);}
        if(!y_condition){currentMovement.setY(currentMovement.getY() + step_y_corrected);}
        if(!z_condition){currentMovement.setZ(currentMovement.getZ() + step_z_corrected);}

        currentMovement.setDuration(delay_ms);
        if(!ended){output.push_back(Movement(currentMovement));};



    }



    if(!output.at(output.size()-1).positionMatches(end)){output.push_back(end);}

    return output;



}



std::vector<Movement> Sequence::generateSegment(const Movement &begin , const Movement &end , const uint64_t step , const uint64_t delay_ms){
    return Sequence::generateSegment(begin , end , step , step , step , delay_ms);
}


std::vector<Movement> Sequence::generateSegment(const Movement &begin , const Movement &end , const uint64_t step){
    return Sequence::generateSegment(begin , end , step , step , step , 100);

}


std::vector<Movement> Sequence::generateSegment(const Movement &begin , const Movement &end){
    return Sequence::generateSegment(begin , end , 1 , 1 , 1 , 100);
}

/**************************************************************/


bool Sequence::operator ==(const Sequence& seq) const{return equals(*this , seq);}
bool Sequence::equals (const Sequence &seq1 , const Sequence &seq2){return std::addressof(seq1) == std::addressof(seq2);}
bool Sequence::isEquivalent(const Sequence &seq) const{

    if(seq.movements.size() != movements.size()){return false;}

    for(unsigned int index = 0 ; index < movements.size() ; index++){
        if(!movements.at(index).isEquivalent(seq.movements.at(index))){return false;}
    }


    return true;
}


