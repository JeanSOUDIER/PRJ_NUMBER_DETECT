#include <iostream>
#include "Robot/Arm.hpp"
#include "Robot/MobileBase.hpp"
#include "Robot/Bluetooth.hpp"
#include "Robot/Lidar.hpp"
#include "Robot/Sequencer.hpp"

int main() {
    // Declare USB ports
    constexpr int ttyUSB_ARBO = 38; // Arm
    constexpr int ttyUSB_LDS = 39;  // Lidar
    constexpr int ttyACM0 = 24;     // Mobile Base
    constexpr int ttyS0 = 0;        // Bluetooth

    int test;
    std::cout << "learn mode ? ";
    std::cin >> test;
    std::cout <<"----SETUP----" << std::endl;
    if(test != 1) { // Mode to play the main program
        Sequencer Prgm(
            new Arm(6, ttyUSB_ARBO, 115200),
            new Bluetooth(ttyS0, 9600),
            new MobileBase(ttyACM0, 115200, new Lidar(ttyUSB_LDS, 230400))
        ); // Initialisation of the robot

        std::cout <<"----LOOP----" << std::endl;
        while(Prgm.Execute()) {} // Main loop of the program
    } else { // Mode to learn new drawing pattern
        MobileBase OpenCR(ttyACM0, 115200, new Lidar(ttyUSB_LDS, 230400)); // Use to place the robot
        delay(1000);

        Arm WidowXL(6, ttyUSB_ARBO, 115200); // Use to draw
        WidowXL.ToKeyboard(false); // function to draw with keyboard
    }

    return 0;
}
