#ifndef MOBILEBASE_H
#define MOBILEBASE_H

#include <iostream>
#include <cmath>
#include <complex>
#include <vector>
#include "Lidar.hpp"
#include "Usb.hpp"

constexpr int ERREUR_STATIQUE = 2;

class MobileBase {
	public:
		explicit MobileBase(const int nb_usb, const int bdrate);
		explicit MobileBase(const double posX, const double posY, const double angle, const int nb_usb, const int bdrate);
		explicit MobileBase(const int nb_usb, const int bdrate, Lidar* RPLidar);
		explicit MobileBase(const double posX, const double posY, const double angle, const int nb_usb, const int bdrate, Lidar* RPLidar);
		~MobileBase();

		void Go(const int x, const int y);
		double getDistBoard();
		void SetMot();
		void SetMotBalance(const double rho, const double theta);
		void SetSpeed(int L, int R);
	private:
		void GetLidarPoints();
		void GetPosBase();
		std::vector<double> FindSegment(int start, int end);

		int m_port_nr;
        int m_bdrate;
        bool m_lidar_start;

        Usb *m_usb;
		Lidar *m_RPLidar;

        pthread_t *inc_x_thread;

		std::vector<double> m_x;
		std::vector<double> m_y;

		double m_posX = 0;
		double m_posY = 0;
		double m_angle = 0;
		double m_dist_board = 0;
};

#endif //MOBILEBASE_H
