#include "Bluetooth.hpp"

Bluetooth::Bluetooth(const int nb_usb, const int bdrate) {
	system("sudo rfcomm watch hci0");
	m_usb = new Usb(nb_usb, bdrate);
	m_port_nr = nb_usb;
	m_bdrate = bdrate;
}

Bluetooth::~Bluetooth() {
	delete m_usb;
}

std::vector<unsigned char> Bluetooth::Read() {
	std::vector<unsigned char> reading;
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
}

void Bluetooth::WriteEnd() {
	std::vector<char> sending;
	sending[0] = 253;
	m_usb->SendBytes(sending);
}
