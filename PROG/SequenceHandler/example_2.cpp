#include "sequencehandler.h"

int main(){

     SequenceHandler seqHandler;

     seqHandler.addSequence('0' , "C:/Sequences/seq0.xml"); //Add Sequence for '0' and bind this char to the sequence from "C:/Sequences/seq0.xml"
     seqHandler.addSequence('1' , "C:/Sequences/seq1.csv");


     const Sequence seq_0 = seqHandler.find('0'); //Sequence from 0
     const std::vector<Movement> movements_0 = seq_0.getMovements_STD(); //Movements of seq_0 in a std::vector

     const Sequence seq_1 = seqHandler.find('1');//Sequence from 1
     const QVector<Movement> movement_1 = seq_1.getMovements();//Movements of seq_1 in a std::vector

     Sequence seqH = seqHandler.find('h'); //Empty sequence, was not added using addSequence.


     for(unsigned int index = 0 ; index < movements_0.size() ; index++){

         const Movement currentMovement = movements_0.at(index); //Current movement
         const Movement nextMovement = ((index+1 < movements_0.size()) ? movements_0.at(index+1): Movement()); //Next one, put to nullptr if index is invalid

         const long int x = currentMovement.getCoordiate(Coordinate::X); //X = 0 i.e. Coordiate::X = static_cast<Coordinate>(0)
         const long int y =currentMovement.getCoordiate(Coordinate::Y); //Y = 1
         const long int z = currentMovement.getCoordiate(Coordinate::Z); //Z = 1
         const uint64_t duration_ms = currentMovement.getDuration();

                 /***************************************************************************/
                            //Do something here with the movement information above
                 /****************************************************************************/

         if(index+1 < movements_0.size()){ //Only for valid instances of nextMovement

             if(currentMovement.isEquivalent(nextMovement)){/*...*/} //If the two movements are equivalent, do stuff
             else if(currentMovement.positionMatches(nextMovement)){/*...*/}
             else{


                 std::vector<Movement> segment = Sequence::generateSegment_STD(currentMovement , nextMovement , 1 , 5 , 3 , 10); //Generate a std::vector<Movement>
                 //...

             }

         }


     }

     /***********************************************************************************/

     for(int index =  0 ; index < movement_1.size() ; index++){

         //Do analog stuff for movements_1

     }






    return 0;
}
