#include "CtrlArm.hpp"

Arm::Arm(int nb, int nb_usb, int bdrate) {
	int lim_min[nb], lim_max[nb];
	for(int i=0;i<nb;i++) {
		lim_min[i] = 0;
		lim_max[i] = 0;
	}
	Arm(nb,nb_usb,bdrate,lim_min,lim_max,5000);
}

Arm::Arm(int nb, int nb_usb, int bdrate, int time) {
	int lim_min[nb], lim_max[nb];
	for(int i=0;i<nb;i++) {
		lim_min[i] = 0;
		lim_max[i] = 0;
	}
	Arm(nb,nb_usb,bdrate,lim_min,lim_max,time);
}

Arm::Arm(int nb, int nb_usb, int bdrate, int *lim_min, int *lim_max) {
	Arm(nb,nb_usb,bdrate,lim_min,lim_max,5000);
}

Arm::Arm(int nb, int nb_usb, int bdrate, int *lim_min, int *lim_max, int time) {
	Usb usb(nb_usb, bdrate);
	m_usb = usb;
	m_nb = nb;
	m_PosArm = new int[nb];
	m_LimMinArm = new int[nb];
	m_LimMaxArm = new int[nb];
	for(int i=0;i<nb;i++) {
		m_PosArm[i] = (lim_min[i]+lim_max[i])/2;
		m_LimMinArm[i] = lim_min[i];
		m_LimMaxArm[i] = lim_max[i];
	}
	m_TimeArm = time;
	if(m_usb.GetActive()) {
		char send_nb[] = {(unsigned char)(nb)};
		Send(ARB_SIZE_POSE, send_nb);
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

void Arm::MoveArm(bool delais) {
	if(m_usb.GetActive()) {
		//sebd pos
		char posi[2*sizeof(m_PosArm)+1];
		posi[0] = 0;
		for(int i=0;i<sizeof(m_PosArm);i++) {
			posi[2*i] = (m_PosArm[i]%256);
			posi[2*i+1] = (unsigned char)(m_PosArm[i]/256);
		}
		Send(ARB_LOAD_POSE, posi);
		//send speed (time)
		char time[] = {0, (m_TimeArm%256), (unsigned char)(m_TimeArm/256), 255, (m_TimeArm%256), (unsigned char)(m_TimeArm/256)};
		Send(ARB_LOAD_SEQ, time);
		//send play sequence
		Send(ARB_PLAY_SEQ, {});
		if(delay) {
			delay(delais+0.2);
		}
	} else {
		std::cout << "port not open" << std::endl;
	}
}

void Arm::Send(int ins, char *data) {
	int sum = 0;
	char send[5+sizeof(data)];
	send[0] = 255;
	send[1] = 253;
	send[2] = sizeof(data)+2;
	send[3] = ins;
	for(int i=0;i<sizeof(data);i++) {
		send[i+4] = data[i];
		sum += data[i];
	}
	send[sizeof(data)+4] = 255-((sum%256)+1);
	m_usb.SendBytes(send);
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
				SetAxePos(5, (int)(gamma));
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

int Arm::GetTime(void) {
	return m_TimeArm;
}

void Arm::SetTime(int time) {
	m_TimeArm = time;
}

int Arm::GetNbMot(void) {
	return m_nb;
}