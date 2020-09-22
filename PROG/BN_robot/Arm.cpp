#include "Arm.hpp"

Arm::Arm(const int nb, const int nb_usb, const int bdrate) {
    std::vector<int> lim_min(nb);
    std::vector<int> lim_max(nb);

    std::fill(lim_min.begin(), lim_min.end(), 0);
    std::fill(lim_max.begin(), lim_max.end(), 0);

	Arm(nb,nb_usb,bdrate,lim_min,lim_max,5000);
}

Arm::Arm(const int nb, const int nb_usb,const int bdrate, const int time) {
    std::vector<int> lim_min(nb);
    std::vector<int> lim_max(nb);

    std::fill(lim_min.begin(), lim_min.end(), 0);
    std::fill(lim_max.begin(), lim_max.end(), 0);

	Arm(nb,nb_usb,bdrate,lim_min,lim_max,time);
}

Arm::Arm(const int nb,const  int nb_usb, const int bdrate,const std::vector<int> &lim_min, const std::vector<int> &lim_max) {
    Arm(nb,nb_usb,bdrate,lim_min,lim_max,5000);
}

Arm::Arm(const int nb, const int nb_usb,const  int bdrate, const std::vector<int> &lim_min, const std::vector<int> &lim_max, const int time) {
	m_usb = new Usb(nb_usb, bdrate);
	m_nb = nb;
    m_PosArm = std::vector<int>(nb);
    m_LimMinArm = std::vector<int>(nb);
    m_LimMaxArm = std::vector<int>(nb);
    SetTime(time);

	for(int i=0;i<nb;i++) {
		m_PosArm[i] = (lim_min[i]+lim_max[i])/2;
		m_LimMinArm[i] = lim_min[i];
		m_LimMaxArm[i] = lim_max[i];
	}

    if(m_usb->GetActive()) {
        delay(5000);
        Homing();
	}
}

Arm::~Arm() {delete m_usb;}


void Arm::MoveArm(bool withDelay) {
    if(m_usb->GetActive()) {
    	std::vector<char> sending(2*m_nb+2);
    	sending[0] = (m_TimeArm%256);
    	sending[1] = static_cast<unsigned char>(m_TimeArm/256);
        for(unsigned int i=0;i<m_PosArm.size();i++) {
			sending[2*i+2] = (m_PosArm[i]%256);
            sending[2*i+3] = static_cast<unsigned char>(m_PosArm[i]/256);
		}
		Send(ARB_DO_FULL, sending);
        if(withDelay){delay(m_TimeArm+200);}
    } else {std::cout << "port not open" << std::endl;}
}

void Arm::Send(int ins, const std::vector<char>&data) {
	int sum = 508+data.size()+2+ins;
    std::vector<char>send(5+data.size());
	send[0] = 255;
	send[1] = 253;
    send[2] = data.size()+2;
	send[3] = ins;
    for(unsigned int i=0;i<data.size();i++) {
		send[i+4] = data[i];
		sum += data[i];
	}
    send[data.size()+4] = 255-((sum%256)+1);
    m_usb->SendBytes(send);
}

bool Arm::PlaceArm(double x, double y, double z) {
	bool test = false;
	x -= A4b;
	y -= A4a;
	z *= -1;
	std::cout << x << " " << y << " " << z << std::endl;
	//convet to cartesian
    const double r = sqrt(x*x+y*y);
    double gamma;
    if(y == 0 && x < 0) {
        gamma = 3.1415;
    } else {
        gamma = 2*atan(y/(x+r));
    }
    const double rA3 = r-A3;
    std::cout << gamma << std::endl;
    //test arm not at right
    if(y > 0) {
    	//test distance not in the robot
		if(r > Lr) {
			//test distance no in the bottom
			if(z > Lz) {
				//test arm not to short
				const double d = sqrt(rA3*rA3+z*z);
				if(d < A1+A2) {
					const double thet = acos(z/d);
					const double thetLoi = acos(-(A1*A1+d*d-A2*A2)/(2*d*A1));
					const double theta1 = -thetLoi+thet+A1thet;

					const double thetLoi2 = acos(-(A1*A1+A2*A2-d*d)/(2*A2*A1));
					const double theta2 = M_PI/2-thetLoi2;

					//compute thet1 (2 triangle)
					/*double theta1 = atan(abs(z)/d);
					theta1 += acos((d*d-A1*A1-A2*A2)/(2*A2*A1));//+A1thet;
					std::cout << theta1 << std::endl;
					//compute theta2 (loi cos)
	                const double theta2 = -M_PI+acos((A1*A1-A2*A2-d*d)/(2*d*A2));
	                std::cout << theta2 << std::endl;
					//compute theta3 (x -> P2, triangle P3P2M)
	                const double x2 = A1*cos(theta1);
	                const double m = rA3-x2;
	                const double theta3 = acos(m/A2);*/
	                const double theta3 = -theta1-theta2;
	                std::cout << theta1 << " " << theta2 << " " << theta3 << " r" << std::endl;
					//set axes
	                SetAxePos(1, (gamma));
	                SetAxePos(2, (theta1));
	                SetAxePos(3, (theta2));
	                SetAxePos(4, (theta3));
	                SetAxePos(5, (M_PI/2-gamma));
				} else {
					//std:cout << "arm too short" << std::endl;
				}
			} else {
				std::cout << "in the bottom" << std::endl;
			}
		} else {
			std::cout << "in the robot" << std::endl;
		}
    } else {
    	std::cout << "in right of arm" << std::endl;
    }
	return test;
}

void Arm::WriteOn() {SetAxePosTic(6, m_LimMinArm[5]);}
void Arm::WriteOff() {SetAxePosTic(6, m_LimMaxArm[5]);}
void Arm::Homing() {
	SetAxePos(1, 0);
	SetAxePos(2, 0);
	SetAxePos(3, 0);
	SetAxePos(4, 0);
	SetAxePos(5, 0);
	WriteOff();
	MoveArm(true);
}


void Arm::SetTime(int time) {
	if(time < limTimeMin) {
		time = limTimeMin;
		std::cout << "time arm sat min" << std::endl;
	}
	if(time > limTimeMax) {
		time = limTimeMax;
		std::cout << "time arm sat max" << std::endl;
	}
	m_TimeArm = time;
}
void Arm::SetLimMinAxe(int nb, int lim) {m_LimMinArm[nb-1] = lim;}
void Arm::SetLimMaxAxe(int nb, int lim) {m_LimMaxArm[nb-1] = lim;}
void Arm::SetAxePos(int nb, double pos) {
	if(pos < -M_PI/2) {pos = -M_PI/2;}
	if(pos > M_PI/2) {pos = M_PI/2;}
	int posi = static_cast<int>((((pos+M_PI/2)*(m_LimMaxArm[nb-1]-m_LimMinArm[nb-1]))/M_PI)+m_LimMinArm[nb-1]);
	SetAxePosTic(nb, posi);
}
void Arm::SetAxePosTic(int nb, int pos) {
	if(pos < m_LimMinArm[nb-1]) {pos = m_LimMinArm[nb-1];}
	if(pos > m_LimMaxArm[nb-1]) {pos = m_LimMaxArm[nb-1];}
	m_PosArm[nb-1] = pos;
}
void Arm::SetLimAxe(int nb, int lim_min, int lim_max) {
    m_LimMinArm[nb-1] = lim_min;
    m_LimMaxArm[nb-1] = lim_max;
}

int Arm::GetBdRate(void) {return m_usb->GetBdRate();}
int Arm::GetPortNb(void) {return m_usb->GetPortNb();}
int Arm::GetTime(void) {return m_TimeArm;}
int Arm::GetNbMot(void) {return m_nb;}
int Arm::GetLimMinAxe(int nb) {return m_LimMinArm[nb-1];}
int Arm::GetLimMaxAxe(int nb) {return m_LimMaxArm[nb-1];}
int Arm::GetAxePos(int nb) {return m_PosArm[nb-1];}

