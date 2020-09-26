#include "Bluetooth.hpp"

Bluetooth::Bluetooth(const int nb_usb, const int bdrate)
	: Bluetooth(nb_usb, bdrate, "88:25:83:F0:56:98") {}

Bluetooth::Bluetooth(const int nb_usb, const int bdrate, const std::string addr) {
	if(nb_usb == 26) {
		m_channel = 0;
	} else if(nb_usb == 27) {
		m_channel = 1;
	} else {
		m_channel = -1;
	}
	if(!IsAddrValid(addr)) {
		m_channel = -1;
		std::cout << "erreur addr" << std::endl;
	} else {
		if(m_channel != -1) {
			std::string cmd = "sudo rfcomm bind "+std::to_string(m_channel)+" "+addr;
			system(cmd.c_str());
			delay(1000);
			m_usb = new Usb(nb_usb, bdrate);
			m_port_nr = nb_usb;
			m_bdrate = bdrate;
			m_addr = addr;
		    std::cout << "Bluetooth start on channel : " << m_channel << std::endl;
		} else {
			std::cout << "erreur port com Bluetooth" << std::endl;
		}
	}
}

Bluetooth::~Bluetooth() {
	if(m_channel != -1) {system("sudo rfcomm release 0");}
	delete m_usb;
}

unsigned char Bluetooth::Read() {
	std::vector<unsigned char> reading(1);
	if(m_channel != -1) {
		reading = m_usb->ReadBytes(1);
	} else {
		reading.at(0) = 0;
	}
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
	if(m_channel != -1) {
		m_usb->SendBytes(sending);
	}
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
