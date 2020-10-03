#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include "Usb.hpp"

class Bluetooth {
	public:
		Bluetooth(const int nb_usb, const int bdrate);
		~Bluetooth();
		unsigned char Read();
		void WriteEnd();
		void WriteWord(std::string txt);
		void WriteWord(std::vector<char> txt);
	private:
		bool IsAddrValid(std::string addr);

		int m_port_nr;
        int m_bdrate;

        Usb *m_usb;
};

#endif //BLUETOOTH_H