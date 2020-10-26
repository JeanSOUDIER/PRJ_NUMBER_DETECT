#ifndef TF_H
#define TF_H

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <cstdio>
#include "Matrix.hpp"

class TF {
	public:
		TF();
		~TF();

		ste::Matrix<double> ToNormalize(ste::Matrix<double> img);
		ste::Matrix<bool> ToThreshold(ste::Matrix<double> img, double level);
		ste::Matrix<bool> ToErosion(ste::Matrix<bool> img);
		ste::Matrix<bool> ToErosion(ste::Matrix<bool> img, ste::Matrix<bool> Patern);
		std::vector<std::vector<unsigned int>> GetEdges(ste::Matrix<bool> img);
		std::vector<double> GetPassMAt(std::vector<std::vector<unsigned int>> edges);
		ste::Matrix<double> ToTF(ste::Matrix<double> img, std::vector<double> VectTF);
		void PrintPPM(ste::Matrix<double> img, std::string path);
		void PrintPPM(ste::Matrix<bool> img, std::string path);

	private:

};

#endif //TF_H
