#ifndef MOBILEBASE_H
#define MOBILEBASE_H

#include <iostream>
#include <cmath>
#include <complex>
#include <vector>
#include <limits>
#include <pthread.h>
#include <atomic>
#include "Lidar.hpp"
#include "Usb.hpp"
#include "Regression.hpp"
#include "utility.h"

class MobileBase {
	public:
		explicit MobileBase(const int nb_usb, const int bdrate);
		explicit MobileBase(const double posX, const double posY, const double angle, const int nb_usb, const int bdrate);
		explicit MobileBase(const int nb_usb, const int bdrate, Lidar* RPLidar);
		explicit MobileBase(const double posX, const double posY, const double angle, const int nb_usb, const int bdrate, Lidar* RPLidar);
		~MobileBase();

		void StartPlacing();
		void Go(const double x, const double y, const double a);
		double getDistBoard();
		void GoPos(const double x, const double y, const double a);
		void SetMotBalance(const double rho, const double theta);
		void SetSpeed(int L, int R);
		void GetLidarPoints();

		static void* MobileBaseHelper(void *context);
        void StartThread();
	protected:
        void* ThreadRun();
	private:
		void GetPosBase();
		std::vector<double> FindSegment(int start, int stop);

		const double SPEED_NORM = 8.68;
		const double SPEED_ANGLE = 450.82*SPEED_NORM/M_PI;

		std::atomic<bool> m_start;

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
		double m_posXgoal = 0;
		double m_posYgoal = 0;
		double m_angle_goal = 0;
		double m_dist_board = 0;
};

#endif //MOBILEBASE_H
