#include <iostream>
#include <cmath>
//#include "Arm.hpp"
//#include "MobileBase.hpp"
#include "Bluetooth.hpp"
//#include "Lidar.hpp"
//#include "Sequencer.hpp"
#include "cam.hpp"

int main() {
    std::cout <<"----SETUP----" << std::endl;
    constexpr int ttyUSB_ARBO = 38;
    constexpr int ttyUSB_LDS = 39;
    constexpr int rfcomm0 = 26;
    constexpr int rfcomm1 = 27;
    constexpr int ttyACM0 = 24;
    constexpr int ttyS0 = 0;
    constexpr int ttyTHS1 = 40;

    /*Sequencer Prgm(
        new Arm(6, ttyUSB_ARBO, 115200),
        nullptr,//new Bluetooth(rfcomm0, 115200),
        new MobileBase(ttyACM0, 115200, new Lidar(ttyUSB_LDS, 230400))
    );

    std::cout <<"----LOOP----" << std::endl;
    while(Prgm.Execute()) {}

    Arm WidowXL(6, ttyUSB_ARBO, 115200);
    WidowXL.ToKeyboard();*/


    //MobileBase OpenCR(ttyACM0, 115200, nullptr);
    //OpenCR.GoPos(0,0,M_PI);
	Cam CPI2;
	CPI2.ImgShow();

    /*Bluetooth BLE(ttyTHS1, 9600);
    while(1) {
    	BLE.WriteEnd();
	}*/
    //BLE.WriteEnd();

    //MobileBase OpenCR(ttyACM0, 115200, new Lidar(ttyUSB_LDS, 230400));
    //delay(10000);
    //Lidar RPLidar(true, ttyUSB_LDS, 230400);
    //std::vector<int> range = RPLidar.GetRange();
    //std::vector<int> intensity = RPLidar.GetIntensity();

    //RPLidar.display(true);
    //RPLidar.DisplayGraph();

    return 0;
}
