#include "MotorWheel.hpp"

MotorWheel::MotorWheel(const int nb_usb, const int bdrate) {
    m_usb = new Usb(nb_usb, bdrate);
    m_port_nr = nb_usb;
    m_bdrate = bdrate;
    std::cout << "MotorWheel start" << std::endl;
}

MotorWheel::~MotorWheel() {
    delete m_usb;
}

void MotorWheel::GoPos(const double x, const double y, const double a) {
    const double r = sqrt(x*x+y*y);
        double gamma;
        if(x == 0 && y == 0) {
            gamma = 0;
        } else if(y == 0 && x < 0) {
            gamma = M_PI;
        } else {
            gamma = 2*atan(y/(x+r));
        }
        gamma += M_PI;
        gamma = std::fmod(gamma,2*M_PI);
        gamma -= M_PI;
        if(gamma != 0) { //go to the angle
            SetSpeed(-SPEED_CST*Utility::sign(gamma),SPEED_CST*Utility::sign(gamma));
        delay(std::abs(gamma)*2*SPEED_ANGLE);
        }
    if(r != 0) { //go to the distance
        SetSpeed(SPEED_CST*Utility::sign(r),SPEED_CST*Utility::sign(r));
        std::cout << r*SPEED_NORM << std::endl;
        delay(r*SPEED_NORM);
    }
    if(a != 0) { //go to the end angle
        SetSpeed(-SPEED_CST*Utility::sign(a),SPEED_CST*Utility::sign(a));
        delay(std::abs(a)*SPEED_ANGLE);
    }
    SetSpeed(0, 0);
}

void MotorWheel::SetSpeed(std::vector<int> speed) {
    SetSpeed(speed.at(0), speed.at(1));
}

void MotorWheel::SetSpeed(int L, int R) {
    if(L > 330) {L = 330;std::cout << "speed sat 0" << std::endl;}
    if(L < -330) {L = -330;std::cout << "speed sat 1" << std::endl;}
    if(R > 330) {R = 330;std::cout << "speed sat 2" << std::endl;}
    if(R < -330) {R = -330;std::cout << "speed sat 3" << std::endl;}
    L += 330;
    R += 330;
    unsigned char Lc = static_cast<unsigned char>(L>>8);
    unsigned char Ld = static_cast<unsigned char>(L%256);
    unsigned char Rc = static_cast<unsigned char>(R>>8);
    unsigned char Rd = static_cast<unsigned char>(R%256);
    int cc = Lc+Ld+Rc+Rd;
    unsigned char c = static_cast<unsigned char>(cc%256);
    std::vector<char> sending{static_cast<unsigned char>(255), static_cast<unsigned char>(Rc), static_cast<unsigned char>(Rd),
                              static_cast<unsigned char>(Lc), static_cast<unsigned char>(Ld), static_cast<unsigned char>(c)};
    m_usb->SendBytes(sending);
}
