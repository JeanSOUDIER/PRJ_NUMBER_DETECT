#ifndef ICP_H
#define ICP_H

#include <iostream>
#include <random>
#include <string>
#include <time.h>
#include "utility.h"

class ICP {
	public:
                ICP();
                ICP(const double error, const unsigned int MaxIter, const int mapLen);
		ICP(const double error, const unsigned int MaxIter, const int mapLen, const double MaxX, const double MaxY, const double MaxA);
		~ICP();

		std::vector<double> GetPos(const std::vector<std::vector<double>> DynData, const std::vector<std::vector<double>> StaData);
                void Print(std::vector<std::vector<double>> Map);
                bool SaveImg(std::vector<std::vector<double>> Map, std::string name);
        protected:
                std::vector<double> PasMat(const double x, const double y, const double a);
                std::vector<double> PasMat(const std::vector<double> pos);
                std::vector<std::vector<double>> FullMap(std::vector<std::vector<double>> Points);
                std::vector<double> PdtMat(std::vector<std::vector<double>> A, std::vector<double> B);
	private:
                std::vector<double> m_max(3);
                double m_error;
                unsigned int m_maxIter;
                const int m_mapLen = 1000;
		
};

#endif //ICP_H
