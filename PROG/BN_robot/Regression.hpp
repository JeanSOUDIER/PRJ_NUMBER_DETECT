#ifndef REGRESSION_H
#define REGRESSION_H

#include "utility.h"

#include <iostream>
#include <cmath>
#include <numeric>
#include <vector>
#include <algorithm>

class Regression {
	public:
		Regression();
		~Regression();
		std::vector<double> RegressionLineaire(std::vector<double> xi, std::vector<double> yi);
	private:
		std::vector<double> m_x;
		std::vector<double> m_y;
		double m_a, m_b, m_r;
};

#endif //REGRESSION_H
