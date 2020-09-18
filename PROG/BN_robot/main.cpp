#include <iostream>
#include <cmath>
#include "Arm.hpp"
#include "MobileBase.hpp"
#include "Bluetooth.hpp"
#include "Lidar.hpp"
#include "Sequencer.hpp"

constexpr int ttyUSB_ARBO = 38;
constexpr int ttyUSB_LDS = 39;
constexpr int ttyAMA0 = 22;
constexpr int ttyACM0 = 24;

int main() {
    std::vector<int> lim_min = {0, 0, 0, 0, 0, 0};
    std::vector<int> lim_max = {1000, 1000, 1000, 1000, 1000, 1000};

    //MobileBase Turtlebot(new Lidar(true, ttyUSB_LDS, 230400));//, ttyACM0, 115200);
    Sequencer Prgm(
        new Arm(6, ttyUSB_ARBO, 115200, lim_min, lim_max, 5000),
        new Bluetooth(ttyAMA0, 115200),
        new MobileBase(new Lidar(true, ttyUSB_LDS, 230400))//, ttyACM0, 115200)
    );

    Prgm.Execute();

    /*WidowXL.SetAxePos(1, 500);
    WidowXL.SetAxePos(2, 500);
    WidowXL.MoveArm(true);
    WidowXL.SetAxePos(1, 800);
    WidowXL.MoveArm(true);*/

    /*delay(1000);
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
