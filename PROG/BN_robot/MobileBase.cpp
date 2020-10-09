#include "MobileBase.hpp"

MobileBase::MobileBase(const int nb_usb, const int bdrate)
	: MobileBase(0, 0, 0, nb_usb, bdrate, nullptr) {}

MobileBase::MobileBase(const double posX, const double posY, const double angle, const int nb_usb, const int bdrate)
	: MobileBase(posX, posY, angle, nb_usb, bdrate, nullptr) {}

MobileBase::MobileBase(const int nb_usb, const int bdrate, Lidar *RPLidar)
    : MobileBase(0, 0, 0, nb_usb, bdrate, RPLidar) {}

MobileBase::MobileBase(const double posX, const double posY, const double angle, const int nb_usb, const int bdrate, Lidar *RPLidar) {
	m_usb = new Usb(nb_usb, bdrate);
	if(RPLidar) {
		m_lidar_start = true;
	} else {
		m_lidar_start = false;
	}
	m_port_nr = nb_usb;
	m_bdrate = bdrate;
	m_posX = posX;
	m_posY = posY;
	m_angle = angle;
	m_RPLidar = RPLidar;
	StartPlacing();
	//thread
	if(m_lidar_start) {
		m_RPLidar->StartThread();
	}
    std::cout << "MobileBase start" << std::endl;
}

MobileBase::~MobileBase() {
	if(m_lidar_start) {
		m_RPLidar->SetStart(false);
		delete m_RPLidar;
	}
	delete m_usb;
}

void MobileBase::StartPlacing() {

}

void MobileBase::Go(const double x, const double y, const double a) {
	//TODO asserv lidar + deplacement
	while(std::abs(x-m_posX) > ERREUR_STATIQUE && std::abs(y-m_posY) > ERREUR_STATIQUE) {
		GetLidarPoints();
		GetPosBase();
		GoPos(m_posX, m_posY, m_angle);
	}
}

void MobileBase::GoPos(const double x, const double y, const double a) {
	const double r = sqrt(x*x+y*y);
    double gamma;
    if(x == 0 && y == 0) {
    	gamma = 0;
    } else if(y == 0 && x < 0) {
    	gamma = M_PI;
    } else {
    	gamma = 2*atan(y/(x+r));
    }
    gamma += a+M_PI;
    gamma = std::fmod(gamma,2*M_PI);
    gamma -= M_PI;
    if(gamma != 0) {
    	SetSpeed(50*Utility::sign(gamma),-50*Utility::sign(gamma));
		delay(std::abs(gamma)*SPEED_ANGLE);
    }
	if(r != 0) {
		SetSpeed(50*Utility::sign(r),50*Utility::sign(r));
		delay(r*SPEED_NORM);
	}
	SetSpeed(0, 0);
}

double MobileBase::getDistBoard() {
	return m_dist_board;
}

void MobileBase::GetLidarPoints() {
	if(m_lidar_start) {
		std::vector<double> xP = m_RPLidar->GetXPos();
		std::vector<double> yP = m_RPLidar->GetYPos();
		std::vector<int> range = m_RPLidar->GetRange();
		m_x.clear();
		m_y.clear();
		for(unsigned int i=0;i<xP.size();i++) {
			if(range[i] == 3500) {
				m_x.push_back(std::numeric_limits<double>::infinity());
				m_y.push_back(std::numeric_limits<double>::infinity());
			} else {
				m_x.push_back(xP.at(i));
				m_y.push_back(yP.at(i));
			}
		}
		m_RPLidar->Display(true);
		std::cout << "out : " << m_RPLidar->SaveLidarPoints() << std::endl;
		GetPosBase();
	}
}

void MobileBase::GetPosBase() {
	std::vector<double> res = FindSegment(12,27);
	std::cout << res[0] << " " << res[1] << " " << res[2] << std::endl;
	/*res = FindSegment(910,179);
	std::cout << res[0] << " " << res[1] << " " << res[2] << std::endl;
	res = FindSegment(179,269);
	std::cout << res[0] << " " << res[1] << " " << res[2] << std::endl;
	res = FindSegment(269,359);
	std::cout << res[0] << " " << res[1] << " " << res[2] << std::endl;*/
	//points cardinaux
	//find a*x+b => a,b x4 + dist board
	//set m_posX, m_posY, m_angle, m_dist_board
}

std::vector<double> MobileBase::FindSegment(int start, int stop) {
	std::vector<double> subvectorX;
	std::vector<double> subvectorY;
	if(start < stop) {
		for(int i=start;i<stop;i++) {
			subvectorX.push_back(m_x.at(i));
			subvectorY.push_back(m_y.at(i));
		}
	} else {
		for(unsigned int i=start;i<m_x.size();i++) {
			subvectorX.push_back(m_x.at(i));
			subvectorY.push_back(m_y.at(i));
		}
		for(int i=0;i<stop;i++) {
			subvectorX.push_back(m_x.at(i));
			subvectorY.push_back(m_y.at(i));
		}
	}
	for(unsigned int i=0;i<subvectorX.size();i++) {
		if(std::isinf(subvectorX.at(i))) {
			subvectorX.erase(subvectorX.begin()+i);
			subvectorY.erase(subvectorY.begin()+i);
		}
	}
	std::vector<std::vector<double>> v = {std::vector<double>() , std::vector<double>()};

    for(unsigned int i=0;i<subvectorX.size();i++) {
      v.at(0).push_back(subvectorX.at(i));
      v.at(1).push_back(subvectorY.at(i));
    }

    Utility::writeCSV("graphXYreduit",v,";");
	Regression reg;
	std::vector<double> res = reg.RegressionLineaire(subvectorX,subvectorY);
	return res;
}

void MobileBase::SetMotBalance(const double rho, const double theta) {
	const int facteur = 4;
	SetSpeed(rho+facteur*theta, rho-facteur*theta);
}

void MobileBase::SetSpeed(int L, int R) {
	L *= -1;
	R *= -1;
	if(L > 100) {L = 100;}
	if(L < -100) {L = -100;}
	if(R > 100) {R = 100;}
	if(R < -100) {R = -100;}
	unsigned char Lc = L+128;
	unsigned char Rc = R+128;
	std::vector<char> sending{char(255), static_cast<char>(Rc), static_cast<char>(Lc)};
	m_usb->SendBytes(sending);
}
