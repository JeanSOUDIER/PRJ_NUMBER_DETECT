#ifndef MOBILEBASE_H
#define MOBILEBASE_H

#include "Lidar.hpp"
#include "Usb.hpp"

class MobileBase {
	public:
		explicit MobileBase(const int nb_usb, const int bdrate, Lidar* = nullptr);
		~MobileBase();

		void Go(const int x, const int y);
		int getDistBoard();
	private:
		int m_port_nr;
        int m_bdrate;

        Usb *m_usb;
		Lidar* m_RPLidar;
};

#endif //MOBILEBASE_H