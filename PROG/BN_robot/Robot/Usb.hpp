#ifndef USB_H
#define USB_H

#include <iostream>
#include <vector>
#include <iterator>
#include <string>

#include "rs232.hpp"

/**

                     USB class

    DUHAMEL Erwan (erwanduhamel@outlook.com)
    SOUDIER Jean  (jean.soudier@insa-strasbourg.fr)


    Provides a class for USB communication.

    Features :

    • Send and read data from USB

    Functions :

    • Usb                            | Constructor with arguments nb_usb (the number of the USB port) and baudrate (the baudrate)
    • GetBdRate()                    | Function that returns the baudrate value
    • GetPortNb()                    | Function that returns the port number value
    • SetActive(state)               | Function to activate the USB port
    • GetActive()                    | Function that returns the state of the USB port
    • SendBytes(data)                | Function to send data throw USB port
    • ReadBytes()                    | Function to read data throw USB port

*/

class Usb {
    public:
        Usb();
        Usb(const std::string nb_usb, const int baudrate);
        Usb(const int nb_usb, const int baudrate);
        ~Usb();

        int GetBdRate(void);
        int GetPortNb(void);
        void SetActive(bool state);
        bool GetActive(void);

        void SendBytes(const std::string& s);
        void SendBytes(const std::vector<char> &data);
        std::vector<unsigned char> ReadBytes(const int n);
        int ReadBytes(const int n, unsigned char* buf);
    private:
        bool m_active;
        int m_port_nr;
        int m_bdrate;

};

#endif //USB_H
