#include "MobileBase.hpp"

MobileBase::MobileBase(const int nb_usb, const int bdrate)
	: MobileBase(0, 0, 0, nb_usb, bdrate, nullptr, false) {}

MobileBase::MobileBase(const double posX, const double posY, const double angle, const int nb_usb, const int bdrate)
	: MobileBase(posX, posY, angle, nb_usb, bdrate, nullptr, false) {}

MobileBase::MobileBase(const int nb_usb, const int bdrate, Lidar *RPLidar, const bool lidar)
    : MobileBase(0, 0, 0, nb_usb, bdrate, RPLidar, lidar) {}

MobileBase::MobileBase(const double posX, const double posY, const double angle, const int nb_usb, const int bdrate, Lidar *RPLidar, const bool lidar) {
	m_usb = new Usb(nb_usb, bdrate);
	m_lidar_start = lidar;
	m_RPLidar = RPLidar;
	m_port_nr = nb_usb;
	m_bdrate = bdrate;
	m_posX = posX;
	m_posY = posY;
	m_angle = angle;
	//thread
	if(m_lidar_start) {
		m_RPLidar->SetStart(true);
		inc_x_thread = new pthread_t();
	    const int rcL = pthread_create(inc_x_thread, NULL, &Lidar::LidarHelper, &m_RPLidar);
	    if (rcL) {
	     	std::cout << "Error:unable to create thread Lidar," << rcL << std::endl;
	    }
	}
}

MobileBase::~MobileBase() {
	m_RPLidar->SetStart(false);
    delete inc_x_thread; //Delete first because otherwise the function called in the thread will have undefined behaviour when executed after calling delete on lidar
	delete m_RPLidar;
	delete m_usb;
}

void MobileBase::Go(const int x, const int y) {
	//TODO asserv lidar + deplacement
	while(std::abs(x-m_posX) > ERREUR_STATIQUE && std::abs(y-m_posY) > ERREUR_STATIQUE) {
		GetLidarPoints();
		GetPosBase();
		SetMot();
	}
}

double MobileBase::getDistBoard() {
	return m_dist_board;
}

void MobileBase::GetLidarPoints() {
	if(m_lidar_start) {
		std::vector<int> range = m_RPLidar->GetRange();
		std::vector<int> intensity = m_RPLidar->GetIntensity();
		for(int i=0;i<static_cast<int>(range.size());i++) {
			m_x[i] = static_cast<double>(range[i])*std::cos(static_cast<double>(intensity[i]));
			m_y[i] = static_cast<double>(range[i])*std::sin(static_cast<double>(intensity[i]));
		}
	}
	
}

void MobileBase::GetPosBase() {
	//points cardinaux
	//find a*x+b => a,b x4 + dist board
	//set m_posX, m_posY, m_angle, m_dist_board
}

void MobileBase::SetMot() {

	const std::complex<double> c = std::polar(m_posX, m_posY);
    //const double r = std::abs(c);
    //const double gamma = std::fmod(std::arg(c) - m_angle + M_PI , (2*M_PI)) - M_PI;

    SetMotBalance(std::abs(c),
                  std::fmod(std::arg(c) - m_angle + M_PI , (2*M_PI)) - M_PI);

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
