#ifndef TF_H
#define TF_H

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <cstdio>
#include <cstring>
#include "Matrix.hpp"

class TF {
	public:
		TF();
		~TF();

		ste::Matrix<double> ToProcessed(std::vector<ste::Matrix<char>> img, double level);
		ste::Matrix<double> ToProcessed(ste::Matrix<double> img, double level);
		ste::Matrix<double> ToGrayLevel(std::vector<ste::Matrix<char>> img);
		ste::Matrix<double> ToNormalize(ste::Matrix<double> img);
		ste::Matrix<unsigned char> ToThreshold(ste::Matrix<double> img, double level);
		ste::Matrix<unsigned char> ToInvertion(ste::Matrix<unsigned char> img);
		ste::Matrix<unsigned char> ToErosion(ste::Matrix<unsigned char> img);
		ste::Matrix<unsigned char> ToErosion(ste::Matrix<unsigned char> img, ste::Matrix<unsigned char> Patern);
		std::vector<std::vector<unsigned int>> GetEdges(ste::Matrix<unsigned char> img);
		std::vector<double> GetPassMAt(std::vector<std::vector<unsigned int>> edges);
		ste::Matrix<double> ToTF(ste::Matrix<double> img, std::vector<double> VectTF);
		void PrintPPM(std::vector<ste::Matrix<char>> img, std::string path);
		void PrintPPM(ste::Matrix<double> img, std::string path);
		void PrintPPM(ste::Matrix<unsigned char> img, std::string path);
		std::vector<ste::Matrix<char>> ReadPPM(std::string path);

	private:

};

#endif //TF_H
