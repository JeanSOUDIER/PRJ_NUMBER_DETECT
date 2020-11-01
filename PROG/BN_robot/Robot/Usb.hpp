#ifndef USB_H
#define USB_H

#include <iostream>
#include <vector>
#include <iterator>
#include "../RS232_CPP/rs232.hpp"

class Usb {
	public:
		Usb();
        Usb(const int nb_usb, const int baudrate);
		~Usb();

		int GetBdRate(void);
		int GetPortNb(void);
		void SetActive(bool state);
        bool GetActive(void);

        void SendBytes(const std::vector<char> &data);
        std::vector<char> ReadBytes(const int n);
	private:
        bool m_active;
        int m_port_nr;
        int m_bdrate;

};

#endif //USB_H
