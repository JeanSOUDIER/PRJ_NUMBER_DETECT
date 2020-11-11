#include <iostream>
#include "Camera/Bluetooth.hpp"
#include "Camera/tf.hpp"

int main() {
    std::cout <<"----SETUP----" << std::endl;
    //constexpr int ttyUSB_ARBO = 38;
    //constexpr int ttyUSB_LDS = 39;
    //constexpr int rfcomm0 = 26;
    //constexpr int rfcomm1 = 27;
    //constexpr int ttyACM0 = 24;
    //constexpr int ttyS0 = 0;
    constexpr int ttyTHS1 = 40;

    //TF myTF;
    //myTF.ToProcessed();

    Bluetooth BLE(ttyTHS1, 9600);
    BLE.SetTX("1234");
	
    sleep(1);
    return 0;
}
