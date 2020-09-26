#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <iostream>
#include "Usb.hpp"

class Bluetooth {
	public:
		Bluetooth(const int nb_usb, const int bdrate);
		~Bluetooth();
		//std::vector<unsigned char> Read();
		unsigned char Read();
		void WriteEnd();
	private:
		int m_port_nr;
        int m_bdrate;

        Usb *m_usb;
};

#endif //BLUETOOTH_H