#ifndef ICP_H
#define ICP_H

#include <iostream>
#include <random>
#include <string>
#include <cmath>
#include <string>
#include "utility.h"
#include "Matrix.hpp"
#include "delaunator.hpp"

#define M_PI 3.141592653538979

class ICP {
	public:
            ICP();
            ICP(const double error, const unsigned int MaxIter, const int mapLen);
			ICP(const double error, const unsigned int MaxIter, const int mapLen, const double MaxX, const double MaxY, const double MaxA);
			~ICP();

			std::vector<double> GetPos(const std::vector<std::vector<double>> DynData, const std::vector<std::vector<double>> StaData);
            bool SaveImg(ste::Matrix<double> Map, std::string name);
    protected:
            ste::Matrix<double> PasMat(const double x, const double y, const double a);
            ste::Matrix<double> PasMat(const std::vector<double> pos);
            ste::Matrix<double> FullMap(std::vector<std::vector<double>> Points);
	private:
            std::vector<double> m_max;
            double m_error;
            unsigned int m_maxIter;
            int m_mapLen;

};

#endif //ICP_H
