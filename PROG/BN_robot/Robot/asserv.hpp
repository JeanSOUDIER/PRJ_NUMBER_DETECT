#ifndef ASSERV_H
#define ASSERV_H

#include <iostream>
#include <cmath>
#include <vector>

class asserv {
	public:
		asserv();
		asserv(int Te, double l, double d);
		~asserv();

		int GetTe();
		std::vector<int> Compute(std::vector<double> pos);
		void SetSpeedNorm(double speed);
	private:
		double m_d;
		double m_l;
		int m_Te;

		double m_speedN;

		double m_x, m_y;
		double m_xOld, m_yOld;

		double m_thet;
};

#endif //ASSERV_H
