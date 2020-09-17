#ifndef CTRLARM_H
#define CTRLARM_H

#include <iostream>
#include <complex>
#include <cmath>
#include <wiringPi.h>
#include "UsbArm.hpp"

#define WriteOn() SetAxePos(5, 100)
#define WriteOff() SetAxePos(5, 200)
#define HOMING

#define ARB_SIZE_POSE 7
#define ARB_LOAD_POSE 8
#define ARB_LOAD_SEQ  9
#define ARB_PLAY_SEQ  10
#define ARB_LOOP_SEQ  11

class Arm {
	public:
		Arm(int nb, int nb_usb, int bdrate);
		Arm(int nb, int nb_usb, int bdrate, int time);
		Arm(int nb, int nb_usb, int bdrate, int *lim_min, int *lim_max);
		Arm(int nb, int nb_usb, int bdrate, int *lim_min, int *lim_max, int time);
		~Arm();
		void SetLimAxe(int nb, int lim_min, int lim_max);
		void SetLimMinAxe(int nb, int lim);
		void SetLimMaxAxe(int nb, int lim);
		int GetLimMinAxe(int nb);
		int GetLimMaxAxe(int nb);
		void SetAxePos(int nb, int pos);
		int GetAxePos(int nb);
		void MoveArm(bool delais);
		bool PlaceArm(double x, double y, double z);
		int GetBdRate(void);
		int GetPortNb(void);
		int GetTime(void);
		void SetTime(int time);
		int GetNbMot(void);
	private:
		void Send(int ins, char *data);
		const int Lr = 179, Lz = -236, a1 = 155, a2 = 150, a3 = 0;
		int *m_PosArm, *m_LimMinArm, *m_LimMaxArm;
		bool m_active = false;
		int m_port_nr, m_bdrate, m_TimeArm = 5000, m_nb = 6;
		Usb m_usb;

};

#endif //CTRLARM_H