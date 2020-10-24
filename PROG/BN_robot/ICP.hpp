#ifndef ICP_H
#define ICP_H

#include <iostream>
#include <random>
#include <string>
#include <cmath>
#include <string>
#include <limits>
#include "utility.h"
#include "Matrix.hpp"
#include "vector2.h"
#include "triangle.h"
#include "delaunay.h"
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
            std::vector<ste::Matrix<double>> ICP::svd(ste::Matrix<double> Mat);
            double ICP::median(vector<double> vec);
            //std::vector<std::vector<double>> nearestNeighbor(std::vector<std::vector<double>> tri, std::vector<double> data);
	private:
            std::vector<double> m_max;
            double m_error;
            unsigned int m_maxIter;
            int m_mapLen;

};

#endif //ICP_H
