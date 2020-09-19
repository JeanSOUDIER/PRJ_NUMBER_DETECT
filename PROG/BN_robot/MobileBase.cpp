#include "MobileBase.hpp"

MobileBase::MobileBase(const int nb_usb, const int bdrate, Lidar* RPLidar) {
	m_usb = new Usb(nb_usb, bdrate);
	m_RPLidar = RPLidar;
	m_port_nr = nb_usb;
	m_bdrate = bdrate;
	//thread
	m_RPLidar->GetMutex()->setState(true);
    pthread_t inc_x_thread;
    const int rcL = pthread_create(&inc_x_thread, NULL, &Lidar::LidarHelper, &m_RPLidar);
    if (rcL) {
     	std::cout << "Error:unable to create thread Lidar," << rcL << std::endl;
    }
}

MobileBase::~MobileBase() {
	m_RPLidar->GetMutex()->setState(false);
	delete m_RPLidar;
	delete m_usb;
}

void MobileBase::Go(const int x, const int y) {
	//TODO asserv lidar + deplacement
}

int MobileBase::getDistBoard() {
	//TODO
	return 0;
}