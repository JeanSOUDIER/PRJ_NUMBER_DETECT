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

        std::vector<double> GetPos(const std::vector<std::vector<double>> &DynData, const std::vector<std::vector<double>> &StaData);
        bool SaveImg(const ste::Matrix<double> &Map, const std::string &name);
    protected:
            std::vector<ste::Matrix<double>> svd(const ste::Matrix<double> &Mat);
            double median(std::vector<double> vec);
            std::vector<std::vector<double>> nearestNeighbor(const std::vector<std::vector<double>> &model, const std::vector<std::vector<double>> &data);
            double pdt(const std::vector<double> &a, const std::vector<double> &b);
            double distEucl(const double &a1, const double &a2, double const &b1, const double &b2);
	private:
            double m_error;
            unsigned int m_maxIter;

};

#endif //ICP_H
