#include "Usb.hpp"

Usb::Usb() {
    m_active = false;
}

Usb::Usb(const std::string nb_usb, const int baudrate)
    : Usb(RS232::GetPortnr(nb_usb), baudrate) {}

Usb::Usb(const int nb_usb, const int baudrate) {
    #if OS == OS_WINDOWS
        m_port_nr = nb_usb-1;
    #else
        m_port_nr = nb_usb;
    #endif
    m_bdrate = baudrate;
 
    std::vector<char> mode={'8','N','1',0}; // 8 data bits, no parity, 1 stop bit
  
    if(RS232::OpenComport(m_port_nr, m_bdrate, &mode[0], 0)) {
        std::cout << "Can not open comport\n" << std::endl;
        m_active = false;
    } else {
        m_active = true;
    }
}

Usb::~Usb() {
    RS232::CloseComport(m_port_nr);
}

void Usb::SendBytes(const std::string &s) {
    std::vector<char> v;
    std::copy(s.begin(), s.end(), std::back_inserter(v));
    SendBytes(v);
}

void Usb::SendBytes(const std::vector<char> &data) {
    std::vector<unsigned char> msg(data.size());
    for(unsigned int i=0;i<data.size();i++) {
        msg.at(i) = static_cast<unsigned char>(data.at(i));
    }
    RS232::cputs(m_port_nr, msg);
    RS232::flushRXTX(m_port_nr);
    /*std::copy(data.begin(), data.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    std::copy(data.begin(), data.end(), std::ostream_iterator<char>(std::cout, ""));
    std::cout << std::endl;*/
}

std::vector<unsigned char> Usb::ReadBytes(const int n) {
    std::vector<unsigned char> raw_bytes(n);
    int n_r = RS232::PollComport(m_port_nr, &raw_bytes[0], n);
    std::vector<unsigned char> ret(n_r);
    for(unsigned int i=0;i<ret.size();i++) {
        ret.at(i) = static_cast<unsigned char>(raw_bytes.at(i));
        //if(ret.at(i)) {std::cout << ret.at(i);} else {std::cout << "\0"; }
    }
    return ret;
}

int Usb::ReadBytes(const int n, unsigned char *buf) {
    RS232::PollComport(m_port_nr, buf, n);
    return n;
}

int Usb::GetBdRate(void) {return m_bdrate;}
int Usb::GetPortNb(void) {return m_port_nr;}
bool Usb::GetActive(void) {return m_active;}

void Usb::SetActive(bool state) {m_active = state;}


