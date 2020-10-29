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

    //Arm WidowXL(6, ttyUSB_ARBO, 115200);
    //WidowXL.ToKeyboard(false);


    /*MobileBase OpenCR(ttyACM0, 115200, nullptr);
    for(int i=-330;i<331;i+=10) {
        std::cout << "i " << i << std::endl;
        OpenCR.SetSpeed(i,i);
        delay(500);
        OpenCR.SetSpeed(0,0);
        delay(500);
    }*/
    
    //OpenCR.GoPos(0,0,M_PI);

    //Bluetooth BLE(rfcomm0, 9600);
    //BLE.WriteEnd();
    /*Bluetooth BLE(ttyS0, 9600);
    std::vector<char> temp = {'\0'};
    while(temp.at(0) != 'h') {
        temp = BLE.GetRX();
        if(temp.at(0)) {
            for(unsigned int i=0;i<temp.size();i++) {
                std::cout << temp.at(i) << " ";
            }
            std::cout << std::endl;
        }
    }
    //BLE.SetTX("hello");
    delay(1000);*/

    MobileBase OpenCR(ttyACM0, 115200, new Lidar(ttyUSB_LDS, 230400));
    delay(1000);
    //OpenCR.GetLidarPoints();
    std::cout << "tic" << std::endl;
    OpenCR.PrintPos();
    OpenCR.GoPos(1000,0,0);
    std::cout << "toc" << std::endl;
    OpenCR.PrintPos();
    delay(1000);

    
    //Lidar RPLidar(true, ttyUSB_LDS, 230400);
    //std::vector<int> range = RPLidar.GetRange();
    //std::vector<int> intensity = RPLidar.GetIntensity();

    //RPLidar.display(true);
    //RPLidar.DisplayGraph();

    return 0;
}
