#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include "../Matrix.hpp"
#include "../ICP.hpp"
#include "../utility.h"

int main() {
	std::vector<std::vector<double>> a = Utility::readCSV_DOUBLE("graphXY0.csv",";");
	std::vector<std::vector<double>> b = Utility::readCSV_DOUBLE("graphXY0.csv",";");
	std::cout << "main" << std::endl;
	ICP myICP(0.1, 20, 1000, 120, 0, 0);
	//std::vector<double> r =
	myICP.GetPosMatlab(a, b);
	//std::cout << r.at(0) << " " << r.at(1) << " " << r.at(2) << std::endl;
    return 0;
}
