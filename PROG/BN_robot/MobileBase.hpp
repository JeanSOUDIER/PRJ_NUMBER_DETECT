#ifndef MOBILEBASE_H
#define MOBILEBASE_H

#include <iostream>
#include <cmath>
#include <complex>
#include <vector>
#include <limits>
#include <pthread.h>
#include <atomic>
#include <ctime>
#include "Lidar.hpp"
#include "Usb.hpp"
#include "Regression.hpp"
#include "utility.h"
#include "ICP.hpp"

class MobileBase {
	public:
		explicit MobileBase(const int nb_usb, const int bdrate);
		explicit MobileBase(const double posX, const double posY, const double angle, const int nb_usb, const int bdrate);
		explicit MobileBase(const int nb_usb, const int bdrate, Lidar* RPLidar);
		explicit MobileBase(const double posX, const double posY, const double angle, const int nb_usb, const int bdrate, Lidar* RPLidar);
		~MobileBase();

		void StartPlacing();
		//void Go(const double x, const double y, const double a);
		//double getDistBoard();
		void GoPos(const double x, const double y, const double a);
		//void SetMotBalance(const double rho, const double theta);
		void SetSpeed(int L, int R);
		void SetSpeedCons(double speed);
		double GetSpeedCons();
		void PrintPos();
		std::vector<double> GetPosAct();

		static void* MobileBaseHelper(void *context);
        void StartThread();
	protected:
        void* ThreadRun();
		void GetLidarPoints(bool nb);
	private:
		//void GetPosBase();
		//std::vector<double> FindSegment(int start, int stop);

		const double SPEED_NORM = 6.25;
		const double SPEED_ANGLE = 1800*SPEED_NORM/M_PI;

		std::atomic<bool> m_start;
		std::atomic<int> m_timeCont;

		int m_port_nr;
        int m_bdrate;
        bool m_lidar_start;

        Usb *m_usb;
		Lidar *m_RPLidar;
        pthread_t *inc_x_thread;

		std::vector<double> m_posXlid;
		std::vector<double> m_posYlid;
		std::vector<double> m_posXN1lid;
		std::vector<double> m_posYN1lid;

		double m_posX = 0;
		double m_posY = 0;
		double m_angle = 0;
		double m_posXgoal = 0;
		double m_posYgoal = 0;
		double m_angle_goal = 0;
		double m_dist_board = 0;

		double m_speedNorm = 0;

		std::clock_t m_startTime;
		std::clock_t m_endTime;
};

#endif //MOBILEBASE_H
