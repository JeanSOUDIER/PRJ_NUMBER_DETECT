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
	GoPos(0,0,0);
	//thread
	if(m_lidar_start) {
		//m_RPLidar->SetStart(true);
		inc_x_thread = new pthread_t();
	    const int rcL = pthread_create(inc_x_thread, NULL, &Lidar::LidarHelper, &m_RPLidar);
	    if (rcL) {
	     	std::cout << "Error:unable to create thread Lidar," << rcL << std::endl;
	    }
	}
    std::cout << "MobileBase start" << std::endl;
}

MobileBase::~MobileBase() {
	if(m_lidar_start) {
		m_RPLidar->SetStart(false);
		delete inc_x_thread; //Delete first because otherwise the function called in the thread will have undefined behaviour when executed after calling delete on lidar
		delete m_RPLidar;
	}
	delete m_usb;
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
    	SetSpeed(50*sign(gamma),-50*sign(gamma));
		delay(std::abs(gamma)*SPEED_ANGLE);
    }
	if(r != 0) {
		SetSpeed(50*sign(r),50*sign(r));
		delay(r*SPEED_NORM);
	}
	SetSpeed(0, 0);
}

double MobileBase::getDistBoard() {
	return m_dist_board;
}

void MobileBase::GetLidarPoints() {
	if(m_lidar_start) {
		/*std::vector<int> range = m_RPLidar->GetRange();
		std::vector<int> intensity = m_RPLidar->GetIntensity();
		for(int i=0;i<static_cast<int>(range.size());i++) {
			m_x[i] = static_cast<double>(range[i])*std::cos(static_cast<double>(intensity[i]));
			m_y[i] = static_cast<double>(range[i])*std::sin(static_cast<double>(intensity[i]));
		}*/
		std::cout << "out : " << m_RPLidar->SaveLidarPoints() << std::endl;
		m_RPLidar->Display(true);
	}
	
}

void MobileBase::GetPosBase() {
	//points cardinaux
	//find a*x+b => a,b x4 + dist board
	//set m_posX, m_posY, m_angle, m_dist_board
}

std::vector<double> MobileBase::FindSegment(int start, int end) {
	
	for(int i=start;i<end;i++) {

	}
	std::vector<double> v(2);
	v[0] = 0;
	v[1] = 1;
	return v;
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

int MobileBase::sign(const double test) {
	if(test < 0) {
		return -1;
	} else {
		return 1;
	}
}
