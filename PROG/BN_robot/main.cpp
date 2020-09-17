#include <iostream>
#include "CtrlArm.hpp"

#define ttyUSB_ARBO 38
//ttyUSB_LDS && ttyUSB_ARBO


int main(int argc, char** argv) {
	int lim_min[] = {0, 0, 0, 0, 0, 0}, lim_max[] = {1000, 1000, 1000, 1000, 1000, 1000};
    Arm WidowWL(6, ttyUSB_ARBO, 115200, lim_min, lim_max, 5000);

    WidowWL.SetAxePos(1, 500);
    WidowWL.MoveArm(true);
    
    return 0; 
}