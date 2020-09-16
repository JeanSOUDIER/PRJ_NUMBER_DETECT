#ifndef CTRLARM_H
#define CTRLARM_H

#include <iostream>
#include <complex>
#include <cmath>
#include "UsbArm.hpp"

#define WriteOn() SetAxePos(5, 100)
#define WriteOff() SetAxePos(5, 200)

class Arm {
	public:
		Arm(int nb, int nb_usb, int bdrate);
		Arm(int nb, int nb_usb, int bdrate, int *lim_min, int *lim_max);
		~Arm();
		void SetLimAxe(int nb, int lim_min, int lim_max);
		void SetLimMinAxe(int nb, int lim);
		void SetLimMaxAxe(int nb, int lim);
		int GetLimMinAxe(int nb);
		int GetLimMaxAxe(int nb);
		void SetAxePos(int nb, int pos);
		int GetAxePos(int nb);
		void MoveArm(void);
		bool PlaceArm(double x, double y, double z);
		int GetBdRate(void);
		int GetPortNb(void);
	private:
		const int Lr = 179, Lz = -236, a1 = 155, a2 = 150, a3 = 0;
		int *m_PosArm, *m_LimMinArm, *m_LimMaxArm;
		bool m_active = false;
		int m_port_nr, m_bdrate;
		Usb m_usb;

};

#endif //CTRLARM_H