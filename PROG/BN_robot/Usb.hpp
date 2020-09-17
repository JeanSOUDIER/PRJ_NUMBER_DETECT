#ifndef USBARM_H
#define USBARM_H

#include <iostream>
#include "rs232.h"

class Usb {
	public:
		Usb();
		Usb(int nb_usb, int baudrate);
		~Usb();
		void SendBytes(char *data);
		int GetBdRate(void);
		int GetPortNb(void);
		void SetActive(bool state);
		bool GetActive(void);
	private:
		bool m_active = false;
		int m_port_nr, m_bdrate;

};

#endif //USBARM_H
