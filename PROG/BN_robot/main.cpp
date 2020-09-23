#include <iostream>
#include <cmath>
#include "Arm.hpp"
#include "MobileBase.hpp"
#include "Bluetooth.hpp"
#include "Lidar.hpp"
#include "Sequencer.hpp"

int main() {
    constexpr int ttyUSB_ARBO = 38;
    constexpr int ttyUSB_LDS = 39;
    constexpr int rfcomm0 = 26;
    constexpr int ttyACM0 = 24;

    std::vector<int> lim_min = {700, 2000, 500, 500, 200, 300};
    std::vector<int> lim_max = {2800, 4100, 2600, 2600, 820, 700};

    /*Sequencer Prgm(
        new Arm(6, ttyUSB_ARBO, 115200, lim_min, lim_max, 5000),
        new Bluetooth(ttyAMA0, 115200),
        new MobileBase(ttyACM0, 115200, new Lidar(true, ttyUSB_LDS, 230400))
    );

    Prgm.Execute();*/

    Arm WidowXL(6, ttyUSB_ARBO, 115200, lim_min, lim_max, 1000);
    /*WidowXL.PlaceArm(300, 120, 0);
    WidowXL.WriteOff();
    WidowXL.MoveArm(true);
    WidowXL.WriteOn();
    WidowXL.MoveArm(true);
    WidowXL.PlaceArm(285, 120, -62);
    WidowXL.MoveArm(true);
    WidowXL.PlaceArm(319, 191, -62);
    WidowXL.MoveArm(true);
    WidowXL.PlaceArm(319, 191, 0);
    WidowXL.MoveArm(true);
    WidowXL.PlaceArm(300, 120, 0);
    WidowXL.MoveArm(true);
    WidowXL.WriteOff();
    WidowXL.MoveArm(true);
    WidowXL.PosToMove();
    WidowXL.MoveArm(true);*/

    WidowXL.ToKeyboard();


    //MobileBase OpenCR(0, 0, 0, ttyACM0, 115200, true, ttyUSB_LDS, 230400);
    //OpenCR.SetSpeed(0,0);

    //Bluetooth BLE(rfcomm0, 9600);
    //BLE.WriteEnd();
    
    /*Lidar RPLidar(true, ttyUSB_LDS, 230400);
	delay(1000);
    RPLidar.Poll();
    delay(1000);
    std::vector<int> range = RPLidar.GetRange();
    std::vector<int> intensity = RPLidar.GetIntensity();

    std::vector<std::vector<double>> pos;
    for(unsigned int i=0;i<range.size();i++) {
    		std::cout << range.at(i)*cos(intensity.at(i)) << " " << range.at(i)*sin(intensity.at(i)) << std::endl;
    }*/

    return 0;
}
