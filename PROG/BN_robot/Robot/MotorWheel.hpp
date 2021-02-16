#ifndef MOTORWHEEL_H
#define MOTORWHEEL_H

#include <cmath>
#include <iostream>
#include <vector>

#include "../RS232_CPP/Usb.hpp"

class MotorWheel {
    public:
        explicit MotorWheel(const int nb_usb, const int bdrate);
        ~MotorWheel();

        void GoPos(const double x, const double y, const double a);
        void SetSpeed(std::vector<int> speed);
        void SetSpeed(int L, int R);
    private:
        int m_port_nr;
        int m_bdrate;

        Usb *m_usb;
};

#endif //MOTORWHEEL_H
