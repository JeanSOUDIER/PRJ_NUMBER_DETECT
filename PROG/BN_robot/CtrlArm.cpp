#include "CtrlArm.hpp"

Arm::Arm(int nb, int nb_usb, int bdrate) {
	Usb usb(nb_usb, bdrate);
	m_usb = usb;
	m_PosArm = new int[nb];
	m_LimMinArm = new int[nb];
	m_LimMaxArm = new int[nb];
	for(int i=0;i<nb;i++) {
		m_PosArm[i] = 0;
		m_LimMinArm[i] = 0;
		m_LimMaxArm[i] = 0;
	}
}

Arm::Arm(int nb, int nb_usb, int bdrate, int *lim_min, int *lim_max) {
	Usb usb(nb_usb, bdrate);
	m_usb = usb;
	m_PosArm = new int[nb];
	m_LimMinArm = new int[nb];
	m_LimMaxArm = new int[nb];
	for(int i=0;i<nb;i++) {
		m_PosArm[i] = (lim_min[i]+lim_max[i])/2;
		m_LimMinArm[i] = lim_min[i];
		m_LimMaxArm[i] = lim_max[i];
	}
}

Arm::~Arm() {}

void Arm::SetLimAxe(int nb, int lim_min, int lim_max) {
	m_LimMinArm[nb-1] = lim_min;
	m_LimMaxArm[nb-1] = lim_max;
}

void Arm::SetLimMinAxe(int nb, int lim) {
	m_LimMinArm[nb-1] = lim;
}

void Arm::SetLimMaxAxe(int nb, int lim) {
	m_LimMaxArm[nb-1] = lim;
}

int Arm::GetLimMinAxe(int nb) {
	return m_LimMinArm[nb-1];
}

int Arm::GetLimMaxAxe(int nb) {
	return m_LimMaxArm[nb-1];
}

void Arm::SetAxePos(int nb, int pos) {
	m_PosArm[nb-1] = pos;
}

int Arm::GetAxePos(int nb) {
	return m_PosArm[nb-1];
}

void Arm::MoveArm(void) {
	if(m_usb.GetActive()) {

	} else {
		std::cout << "port not open" << std::endl;
	}
}

bool Arm::PlaceArm(double x, double y, double z) {
	bool test = false;
	//convet to cartesian
	std::complex<double> c = std::polar(x, y);
	double r = abs(c), gamma = arg(c);
	//test distance not in the robot
	if(r > Lr) {
		//test distance no in the bottom
		if(z > Lz) {
			//test arm not to short
			if(sqrt(r*r+z*z) < a1+a2+a3) {
				double d = sqrt((r-a3)*(r-a3)+z*z);
				//compute thet1 (2 triangle)
				double theta1 = atan(abs(z)/d);
				theta1 += acos((d*d-a1*a1-a2*a2)/(2*a2*a1)); //check bigger ?
				//compute theta2 (loi cos)
				double theta2 = M_PI-acos((a1*a1-a2*a2-d*d)/(2*d*a2));
				//compute theta3 (x -> P2, triangle P3P2M)
				double x2 = a1*cos(theta1);
				double m = r-a3-x2;
				double theta3 = acos(m/a2);
				//set axes
				SetAxePos(1, (int)(gamma));
				SetAxePos(2, (int)(theta1));
				SetAxePos(3, (int)(theta2));
				SetAxePos(4, (int)(theta3));
			} else {
				//std:cout << "arm too short" << std::endl;
			}
		} else {
			std::cout << "in the bottom" << std::endl;
		}
	} else {
		std::cout << "in the robot" << std::endl;
	}
	return test;
}

int Arm::GetBdRate(void) {
	return m_usb.GetBdRate();
}

int Arm::GetPortNb(void) {
	return m_usb.GetPortNb();
}
