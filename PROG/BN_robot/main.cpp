#include <iostream>
#include <cmath>
#include "Arm.hpp"
#include "MobileBase.hpp"
#include "Bluetooth.hpp"
#include "Lidar.hpp"
#include "Sequencer.hpp"

int main() {
    std::cout <<"----SETUP----" << std::endl;
    constexpr int ttyUSB_ARBO = 38;
    constexpr int ttyUSB_LDS = 39;
    constexpr int rfcomm0 = 26;
    constexpr int rfcomm1 = 27;
    constexpr int ttyACM0 = 24;
    constexpr int ttyS0 = 0;

    /*Sequencer Prgm(
        new Arm(6, ttyUSB_ARBO, 115200),
        nullptr,//new Bluetooth(rfcomm0, 115200),
        new MobileBase(ttyACM0, 115200, new Lidar(ttyUSB_LDS, 230400))
    );

    std::cout <<"----LOOP----" << std::endl;
    while(Prgm.Execute()) {}*/

    Arm WidowXL(6, ttyUSB_ARBO, 115200);
    WidowXL.ToKeyboard(true);


    //MobileBase OpenCR(ttyACM0, 115200, nullptr);
    //OpenCR.GoPos(0,0,M_PI);

    //Bluetooth BLE(rfcomm0, 9600);
    //BLE.WriteEnd();
    /*Bluetooth BLE(ttyS0, 9600);
    unsigned char temp = '\0';
    while(temp != '1') {
        temp = BLE.Read();
        if(temp) {std::cout << temp << std::endl;}
    }*/

    //std::vector<char> v = {'h','e','l','l','o'};
    //BLE.SendBytes(v);

    //MobileBase OpenCR(ttyACM0, 115200, new Lidar(ttyUSB_LDS, 230400));
    //delay(10000);
    //OpenCR.GetLidarPoints();
    
    //Lidar RPLidar(true, ttyUSB_LDS, 230400);
    //std::vector<int> range = RPLidar.GetRange();
    //std::vector<int> intensity = RPLidar.GetIntensity();

    //RPLidar.display(true);
    //RPLidar.DisplayGraph();

    return 0;
}
