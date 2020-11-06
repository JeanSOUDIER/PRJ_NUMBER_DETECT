#ifndef ASSERV_H
#define ASSERV_H

#include <iostream>
#include <cmath>
#include <vector>

class asserv {
	public:
		asserv();
		asserv(double Te, double l, double d);
		~asserv();

		int GetTe();
		std::vector<int> Compute(std::vector<double> pos);
		void SetSpeedNorm(double speed);
	private:
		const double SPEED_TR = 80;
		const double SPEED_ROT = 1.5;

		double m_d;
		double m_l;
		double m_Te;

		double m_speedN;

		double m_x, m_y;
		double m_xOld, m_yOld;

		double m_thet;
		double m_xM, m_yM;

		double m_ex, m_ey;
};

#endif //ASSERV_H
