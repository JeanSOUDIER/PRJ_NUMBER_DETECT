#include "sequencehandler.h"

#include <vector>

void foo(long int , long int , long int , uint64_t);

int main(){


    SequenceHandler seqHandler('0' , "/Sequences/sequence_0.xml"); //Create a sequence handler with '0' as first item


/*----------------------------------------------------------------------------------------------*/

    std::vector<Movement*> movement_0 = seqHandler.find('0').getMovements_STD(); //Create a vector of movements


    for(unsigned int index = 0 ; index < movement_0.size() ; index++){

        const Movement *currentMovement = movement_0.at(index);
        const Movement nextMovement = *((index+1 < movement_0.size()) ? movement_0.at(index+1): nullptr);

        const long int x = currentMovement->getX();
        const long int y =currentMovement->getY();
        const long int z = currentMovement->getZ();
        const uint64_t duration_ms = currentMovement->getDuration();

        foo(x , y , z , duration_ms); //Do stuff for each movement

        if(index+1 < movement_0.size()){ //Only for valid instances of nextMovement

            if(currentMovement->isEquivalent(nextMovement)){/*...*/}

        }


    }

/*----------------------------------------------------------------------------------------------*/


    seqHandler.addSequence('1' , "/Sequences/sequence_1.xml");


    std::vector<Movement*> movements_1 = seqHandler.find('1').getMovements_STD();


    //Do stuff with movements_1



}

/**********************************************************************************************************/
/*                                                                                                        */
/**********************************************************************************************************/

void foo(long int , long int , long int , uint64_t){/*...*/}
