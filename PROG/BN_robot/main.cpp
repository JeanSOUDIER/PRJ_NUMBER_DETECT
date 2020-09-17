#include <iostream>
#include "Arm.hpp"

constexpr int ttyUSB_ARBO = 38;
//ttyUSB_LDS && ttyUSB_ARBO

int main() {
    std::vector<int> lim_min = {0, 0, 0, 0, 0, 0};
    std::vector<int> lim_max = {1000, 1000, 1000, 1000, 1000, 1000};

    Arm WidowWL(6, ttyUSB_ARBO, 115200, lim_min, lim_max, 5000);

    WidowWL.SetAxePos(1, 500);
    WidowWL.SetAxePos(2, 500);
    WidowWL.MoveArm(true);

    return 0;
}
