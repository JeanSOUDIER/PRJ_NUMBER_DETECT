#include <iostream>
#include <vector>
#include <cmath>
#include "../Matrix.hpp"
#include "../ICP.hpp"
#include "../utility.h"

int main() {
	std::vector<std::vector<double>> a = Utility::readCSV_DOUBLE("graphXY0.csv",";");
	ste::Matrix<double> Sta(a);
	std::vector<std::vector<double>> b = Utility::readCSV_DOUBLE("graphXY1.csv",";");
	ste::Matrix<double> Dyn(b);
	ICP myICP(0.001, 100, 1000, 50, 50, 2*M_PI);
	std::vector<double> r = myICP.GetPos(Dyn, Sta);
	std::copy(r.begin(), r.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << std::endl;
    return 0;
}
