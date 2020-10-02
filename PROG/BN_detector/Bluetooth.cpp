#include "Bluetooth.hpp"

Bluetooth::Bluetooth(const int nb_usb, const int bdrate) {
	m_usb = new Usb(nb_usb, bdrate);
	m_port_nr = nb_usb;
	m_bdrate = bdrate;
}

Bluetooth::~Bluetooth() {
	delete m_usb;
}

unsigned char Bluetooth::Read() {
	std::vector<unsigned char> reading(1);
	reading = m_usb->ReadBytes(1);
	return reading.at(0);
}

/*std::vector<unsigned char> Bluetooth::Read() {
	std::vector<unsigned char> reading(1);
	unsigned char length;
	reading[0] = '\0';
	while(reading[0] != 255) {
		reading = m_usb->ReadBytes(1);
	}
	reading.clear();
	reading = m_usb->ReadBytes(1);
	length = reading[0];
	//lecture
	return m_usb->ReadBytes(length);
}*/

void Bluetooth::WriteEnd() {
	std::vector<char> sending(1);
	sending[0] = '0';
	m_usb->SendBytes(sending);
}

bool Bluetooth::IsAddrValid(std::string addr) {
	if(addr.size() == 17) {
		std::replace(addr.begin(), addr.end(), ':', ' ');  // replace ':' by ' '
		std::stringstream ss(addr);
		int temp;
		bool test = true;
		while (ss >> std::hex >> temp) {
		    if(temp < 0 || temp > 255) {
		    	test = false;
		    	break;
		    }
		}
		return test;
	} else {
		return false;
	}
}
