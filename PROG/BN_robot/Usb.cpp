#include "Usb.hpp"

Usb::Usb() {
	m_active = false;
}

Usb::Usb(const int nb_usb, const int baudrate) {
	m_port_nr = nb_usb; /* /dev/ttyUSB0 */
    m_bdrate = baudrate; /* 115200 baud */
 
    std::vector<char> mode={'8','N','1',0}; // 8 data bits, no parity, 1 stop bit
  
    if(RS232_OpenComport(m_port_nr, m_bdrate, &mode[0], 0)) {
        std::cout << "Can not open comport\n" << std::endl;
        m_active = false;
    } else {
		m_active = true;
    }
}

Usb::~Usb() {}

void Usb::SendBytes(const std::vector<char>&data) {RS232_cputs(m_port_nr, &data[0]);}

int Usb::GetBdRate(void) {return m_bdrate;}
int Usb::GetPortNb(void) {return m_port_nr;}
bool Usb::GetActive(void) {return m_active;}

void Usb::SetActive(bool state) {m_active = state;}


