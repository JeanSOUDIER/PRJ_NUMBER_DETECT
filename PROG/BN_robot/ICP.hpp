#ifndef ICP_H
#define ICP_H

#include <iostream>
#include <random>
#include <string>
#include <cmath>
#include <math.h>
#include <string>
#include <limits>
#include "utility.h"
#include "Matrix.hpp"

class ICP {
	public:
        ICP();
		ICP(const double error, const unsigned int MaxIter);
		~ICP();

        std::vector<double> GetPos(const std::vector<std::vector<double>> DynData, const std::vector<std::vector<double>> StaData);
        bool SaveImg(ste::Matrix<double> Map, std::string name);
    protected:
            std::vector<ste::Matrix<double>> svd(ste::Matrix<double> Mat);
            double median(std::vector<double> vec);
            std::vector<std::vector<double>> nearestNeighbor(std::vector<std::vector<double>> model, std::vector<std::vector<double>> data);
            double pdt(std::vector<double> a, std::vector<double> b);
            double distEucl(double a1, double a2, double b1, double b2);
	private:
            double m_error;
            unsigned int m_maxIter;

};

#endif //ICP_H
