#ifndef ICP_H
#define ICP_H

#include <iostream>
#include <random>
#include <string>
#include <cmath>
#include <math.h>
#include <string>
#include <limits>
#include "../SequenceHandler_STL/utility.h"
#include "../Matrix/Matrix.hpp"

/**

                     ICP class

    DUHAMEL Erwan (erwanduhamel@outlook.com)
    SOUDIER Jean  (jean.soudier@insa-strasbourg.fr)


    Provides a class for ICP (Iterative Closest Point) computing.

    Features :

    • Compute an ICP between 2 frames

    Functions :

    • ICP                           | Constructor with arguments (error and maxIter) to break the iterative loop
    • GetPos(DynData,StaData)       | Function that computes the paasage matrix. The output is <Tx, Ty, Theta> and the inputs are the 2 clouds of Lidar-obtained points (size(N*2))
    • SaveImg(Map,name)             | Function that writes a Map in a csv file.
    • svd()                         | Function that computes the SVD (Singular Value Decomposition) of a 2x2 matrix.
    • median()                      | Function that computes the median of a vector.
    • nearestNeighbor()             | Function that computes the nearest neighbor of each point of 2 clouds of points (size(N*2))
    • pdt()                         | Function that returns the product of 2 vectors
    • distEucl()                    | Function that returns the euclidan distance of 2 points (x0,y0,x1,y1) were the first point is (x0,y0) and the second is (x1,y1)

*/

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
