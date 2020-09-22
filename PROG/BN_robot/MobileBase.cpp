#include "MobileBase.hpp"

MobileBase::MobileBase(const int nb_usb, const int bdrate, const int lidar_nb_usb, const int lidar_bdrate) {
	MobileBase(0, 0, 0, nb_usb, bdrate, true, lidar_nb_usb, lidar_bdrate);
}

MobileBase::MobileBase(const int nb_usb, const int bdrate, const int lidar_start, const int lidar_nb_usb, const int lidar_bdrate) {
	MobileBase(0, 0, 0, nb_usb, bdrate, lidar_start, lidar_nb_usb, lidar_bdrate);
}

MobileBase::MobileBase(const double posX, const double posY, const double angle, const int nb_usb, const int bdrate, const int lidar_nb_usb, const int lidar_bdrate) {
	MobileBase(posX, posY, angle, nb_usb, bdrate, true, lidar_nb_usb, lidar_bdrate);
}

MobileBase::MobileBase(const double posX, const double posY, const double angle, const int nb_usb, const int bdrate, const int lidar_start, const int lidar_nb_usb, const int lidar_bdrate) {
	m_usb = new Usb(nb_usb, bdrate);
	m_RPLidar = new Lidar(lidar_start, lidar_nb_usb, lidar_bdrate);
	m_port_nr = nb_usb;
	m_bdrate = bdrate;
	m_posX = posX;
	m_posY = posY;
	m_angle = angle;
	//thread
	m_RPLidar->GetMutex()->setState(true);
    pthread_t inc_x_thread;
    /*const int rcL = pthread_create(&inc_x_thread, NULL, &Lidar::LidarHelper, &m_RPLidar);
    if (rcL) {
     	std::cout << "Error:unable to create thread Lidar," << rcL << std::endl;
    }*/
}

MobileBase::~MobileBase() {
	m_RPLidar->GetMutex()->setState(false);
	delete m_RPLidar;
	delete m_usb;
}

void MobileBase::Go(const int x, const int y) {
	//TODO asserv lidar + deplacement
	while(abs(x-m_posX) > ERREUR_STATIQUE && abs(y-m_posY) > ERREUR_STATIQUE) {
		GetLidarPoints();
		GetPosBase();
		SetMot();
	}
}

double MobileBase::getDistBoard() {
	return m_dist_board;
}

void MobileBase::GetLidarPoints(void) {
	std::vector<int> range = m_RPLidar->load(std::memoy_order_aquire)->GetRange();
	std::vector<int> intensity = m_RPLidar->load(std::memoy_order_aquire)->GetIntensity();
	for(int i=0;i<static_cast<int>(range.size());i++) {
		m_x[i] = static_cast<double>(range[i])*cos(static_cast<double>(intensity[i]));
		m_y[i] = static_cast<double>(range[i])*sin(static_cast<double>(intensity[i]));
	}
}

void MobileBase::GetPosBase(void) {
	//points cardinaux
	//find a*x+b => a,b x4 + dist board
	//set m_posX, m_posY, m_angle, m_dist_board
}

void MobileBase::SetMot(void) {
	const std::complex<double> c = std::polar(m_posX, m_posY);
    const double r = abs(c);
    double gamma = arg(c);
    gamma = gamma-m_angle+M_PI;
    gamma = Modulo(gamma,(2*M_PI));
    gamma -= M_PI;
    SetMotBalance(r, gamma);

}

double MobileBase::Modulo(double n, double m) {
	while(n > m) {
		n -= m;
	}
	return n;
}

std::vector<double> MobileBase::FindSegment(int start, int end) {
	
	for(int i=start;i<end;i++) {

	}
	std::vector<double> v(2);
	v[0] = 0;
	v[1] = 1;
	return v;
}

void MobileBase::SetMotBalance(double rho, double theta) {
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
	std::vector<char> sending{255, static_cast<char>(Rc), static_cast<char>(Lc)};
	m_usb->SendBytes(sending);
}
