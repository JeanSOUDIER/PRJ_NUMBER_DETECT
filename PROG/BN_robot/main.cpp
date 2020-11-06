#include <iostream>
#include <cmath>
#include "Robot/Arm.hpp"
#include "Robot/MobileBase.hpp"
#include "Robot/Bluetooth.hpp"
#include "Robot/Lidar.hpp"
#include "Robot/Sequencer.hpp"

int main() {
    constexpr int ttyUSB_ARBO = 38;
    constexpr int ttyUSB_LDS = 39;
    constexpr int ttyACM0 = 24;
    constexpr int ttyS0 = 0;

    int test;
    std::cout << "learn mode ? ";
    std::cin >> test;
    std::cout <<"----SETUP----" << std::endl;
    if(test != 1) {
        Sequencer Prgm(
            new Arm(6, ttyUSB_ARBO, 115200),
            new Bluetooth(ttyS0, 9600),
            new MobileBase(ttyACM0, 115200, new Lidar(ttyUSB_LDS, 230400))
        );

        std::cout <<"----LOOP----" << std::endl;
        while(Prgm.Execute()) {}
    } else {
        MobileBase OpenCR(ttyACM0, 115200, new Lidar(ttyUSB_LDS, 230400));
        delay(1000);

        Arm WidowXL(6, ttyUSB_ARBO, 115200);
        WidowXL.ToKeyboard(false);
    }

    return 0;
}
