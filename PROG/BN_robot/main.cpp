#include <iostream>
#include "Arm.hpp"
#include "Lidar.hpp"

constexpr int ttyUSB_ARBO = 38;
constexpr int ttyUSB_LDS = 39;

int main() {
    std::vector<int> lim_min = {0, 0, 0, 0, 0, 0};
    std::vector<int> lim_max = {1000, 1000, 1000, 1000, 1000, 1000};

    Arm WidowWL(6, ttyUSB_ARBO, 115200, lim_min, lim_max, 5000);
    Lidar RPLidar(true, ttyUSB_LDS,230400);

    WidowWL.SetAxePos(1, 500);
    WidowWL.SetAxePos(2, 500);
    WidowWL.MoveArm(true);

    RPLidar.Poll();
    delay(1000);
    std::vector<int> data = RPLidar.GetRange();
    std::copy(data.begin(), data.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << std::endl;
	data = RPLidar.GetIntensity();
    std::copy(data.begin(), data.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << std::endl;

    return 0;
}
