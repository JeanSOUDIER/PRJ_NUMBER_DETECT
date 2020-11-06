#include "asserv.hpp"

asserv::asserv()
	: asserv(0.02,10,5) {}

asserv::asserv(double Te, double l, double d) {
	m_Te = Te;
	m_l = l;
	m_d = d;
	m_x = 0;
	m_y = 0;
	m_thet = -M_PI/2;
	m_xM = 0;
	m_yM = 0;
	std::cout << "asserv start" << std::endl;
}

asserv::~asserv() {}

void asserv::SetSpeedNorm(double speed) {
	m_speedN = speed;
	if(!m_speedN) {
		//m_thet = 0;
	}
}

int asserv::GetTe() {
	return m_Te*1000;
}

std::vector<int> asserv::Compute(std::vector<double> pos) {
	std::vector<double> in = {m_xM, m_yM, m_thet};

	std::cout << "data x " << in.at(0) << " y " << in.at(1) << " thet " << in.at(2) << std::endl;
	//compute consigne
	m_xOld = m_x;
	m_yOld = m_y;
	double delta = 0.1;
	if(m_xOld <= delta && m_yOld < m_l && m_yOld >= 0) {
		std::cout << "1" << std::endl;
		m_x = 0;
		m_y = m_yOld + m_Te*m_speedN;
	} else if(m_xOld < m_d+delta && m_xOld > -delta && m_yOld >= m_l) {
		std::cout << "2" << std::endl;
		m_x = m_xOld + m_Te*m_speedN;
		m_y = m_l + (m_d/2)*std::sin(M_PI*m_x/m_d);
	} else if(m_xOld >= m_d-delta && m_yOld >= 0 && m_yOld < m_l) {
		std::cout << "3" << std::endl;
		m_x = m_d;
		m_y = m_yOld - m_Te*m_speedN;
	} else if(m_xOld > -delta && m_xOld < m_d+delta && m_yOld <= 0) {
		std::cout << "4" << std::endl;
		m_x = m_xOld - m_Te*m_speedN;
		m_y = -(m_d/2)*std::sin(M_PI*m_x/m_d);
		if(m_xOld-m_Te*m_speedN < 0) {std::cout << "pb part 4" << std::endl;}
	} else {
		std::cout << "no consigne" << std::endl;
		m_x = 0;
		m_y = 0;
	}
	//compute artan2
	double vxR = (m_x-m_xOld)/m_Te;
	double vyR = (m_y-m_yOld)/m_Te;
	double thetaR = std::fmod(std::atan2(vyR, vxR),2*M_PI);
	std::cout << "cons x " << m_x << " y " << m_y << " thet " << thetaR << std::endl;
	//compute error
	double ex = m_x-in.at(0);
	double ey = m_y-in.at(1);
	double etheta = thetaR-in.at(2);
	std::cout << "err x " << ex << " y " << ey << " thet " << etheta << std::endl;
	//compute ctrl
	double L = std::sqrt(ex*ex+ey*ey);
	double alpha = std::fmod(std::atan2(ey,ex),2*M_PI)-in.at(2);
	double beta = etheta-alpha;
	std::cout << "coeff a " << alpha << " b " << beta << " L " << L << std::endl;
	//compute u/w
	double v = 2.1*L;
	double w = 50*alpha-beta;
	std::cout << "speed P v " << v << " w " << w << std::endl;
	//compute speed
	std::cout << "m_thet " << m_thet << std::endl;
	std::vector<int> speed(2);
	double vx = v*std::cos(m_thet);
	double vy = v*std::sin(m_thet);

	double vr, vl;
	if(m_yM > m_l/2) {
		vr = 80*v*(1+1.4*std::cos(m_thet));
		vl = 80*v*(1-1.4*std::cos(m_thet));
	} else {
		vr = 80*v*(1-1.4*std::cos(m_thet));
		vl = 80*v*(1+1.4*std::cos(m_thet));
	}
	std::cout << "speed vr " << vr << " vl " << vl << std::endl << std::endl;
	speed.at(0) = static_cast<int>(vr);
	speed.at(1) = static_cast<int>(vl);
	
	std::cout << "speed vx " << vx << " vy " << vy << std::endl << std::endl;

	m_xM += vx*m_Te;
	m_yM += vy*m_Te;
	m_thet += w*m_Te;

	return static_cast<std::vector<int>>(speed);
}
