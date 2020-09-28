#include "Arm.hpp"

Arm::Arm(const int nb, const int nb_usb, const int bdrate)
    : Arm(nb,nb_usb,bdrate,{700, 2000, 500, 500, 200, 300},{2800, 4100, 2600, 2600, 820, 700},5000){}

Arm::Arm(const int nb, const int nb_usb,const int bdrate, const int time)
    : Arm(nb,nb_usb,bdrate,{700, 2000, 500, 500, 200, 300},{2800, 4100, 2600, 2600, 820, 700},time) {}

Arm::Arm(const int nb,const  int nb_usb, const int bdrate,const std::vector<int> &lim_min, const std::vector<int> &lim_max)
    : Arm(nb,nb_usb,bdrate,lim_min,lim_max,5000) {}

Arm::Arm(const int nb, const int nb_usb,const  int bdrate, const std::vector<int> &lim_min, const std::vector<int> &lim_max, const int time) {

    m_usb = new Usb(nb_usb, bdrate);
    m_nb = nb;
    m_PosArm = std::vector<int>(nb);
    m_LimMinArm = std::vector<int>(nb);
    m_LimMaxArm = std::vector<int>(nb);

    for(int i=0 ; i<nb ; i++) {
        m_PosArm[i] = (lim_min[i]+lim_max[i])/2;
        m_LimMinArm[i] = lim_min[i];
        m_LimMaxArm[i] = lim_max[i];
    }

    if(m_usb->GetActive()) {
        delay(10000);
        PosToMove();
        MoveArm(true);
        PosToMove();
        MoveArm(true);
    }

    SetTime(time);
    std::cout << "Arm start" << std::endl;
}

Arm::~Arm() {delete m_usb;}


void Arm::MoveArm(bool withDelay) {
    if(m_usb->GetActive()) {
        std::vector<char> sending(2*m_nb+2);
        sending[0] = (m_TimeArm%256);
        sending[1] = static_cast<unsigned char>(m_TimeArm/256);
        for(unsigned int i=0;i<m_PosArm.size();i++) {
            sending[2*i+2] = (m_PosArm[i]%256);
            sending[2*i+3] = static_cast<unsigned char>(m_PosArm[i]/256);
        }
        Send(ARB_DO_FULL, sending);
        if(withDelay){delay(m_TimeArm+50);}
    } else {std::cout << "port not open" << std::endl;}
}

void Arm::Send(int ins, const std::vector<char>&data) {
    int sum = 508+data.size()+2+ins;
    std::vector<char>send(5+data.size());
    send[0] = char(255);
    send[1] = char(253);
    send[2] = char(data.size()+2);
    send[3] = char(ins);
    for(unsigned int i=0;i<data.size();i++) {
        send[i+4] = data[i];
        sum += data[i];
    }
    send[data.size()+4] = char(255-((sum%256)+1));
    m_usb->SendBytes(send);
}

bool Arm::PlaceArm(double x, double y, double z) {
    std::cout << "x = " << x << ", y = " << y << ", z = " << z << std::endl;
    bool test = false;
    x -= A4b;
    y -= A4a;
    z *= -1;
    //convet to cartesian
    const double r = sqrt(x*x+y*y);
    const double gamma = (y == 0 && x < 0) ? 3.1415 : 2*atan(y/(x+r));

    const double rA3 = r-A3;
    std::cout << gamma << std::endl;
    //test arm not at right
    if(y > 0) {
        //test distance not in the robot
        if(r > Lr) {
            //test distance no in the bottom
            if(z > Lz) {
                //test arm not to short
                const double d = sqrt(rA3*rA3+z*z);
                if(d < A1+A2) {
                    const double thet = acos(z/d);
                    const double thetLoi = acos(-(A1*A1+d*d-A2*A2)/(2*d*A1));
                    const double theta1 = -thetLoi+thet+A1thet;

                    const double thetLoi2 = acos(-(A1*A1+A2*A2-d*d)/(2*A2*A1));
                    const double theta2 = M_PI/2-thetLoi2;

                    const double theta3 = -theta1-theta2;
                    //set axes
                    SetAxePos(1, (gamma));
                    SetAxePos(2, (theta1));
                    SetAxePos(3, (theta2));
                    SetAxePos(4, (theta3));
                    SetAxePos(5, (M_PI/2-gamma));
                } else {
                    //std:cout << "arm too short" << std::endl;
                }
            } else {
                //std:cout << "arm too short" << std::endl;
            }
        } else {
            std::cout << "in the bottom" << std::endl;
        }
    } else {
        std::cout << "in the robot" << std::endl;
    }
    return test;
}

char Arm::getch() {

    char buf = 0;
    struct termios old = {0 , 0 , 0 ,0 , 0 , {0}};

    if (tcgetattr(0, &old) < 0){perror("tcsetattr()");}

    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;

    if (tcsetattr(0, TCSANOW, &old) < 0){perror("tcsetattr ICANON");}

    if (read(0, &buf, 1) < 0){ perror ("read()");}

    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;

    if (tcsetattr(0, TCSADRAIN, &old) < 0){perror ("tcsetattr ~ICANON");}

    return buf;
}

void Arm::ToKeyboard(void) {

    int x = 300 ;
    int y = 120;
    int z = 0;

    int pas = 10;
    SetTime(1000);
    PlaceArm(x, y, z);
    WriteOn();
    MoveArm(true);
    SetTime(0);
    std::cout << "Entrer your caractere : ";
    char caractere = getchar();
    std::cout << "Votre caractere est : " << caractere << std::endl;
    std::vector<Movement> move;
    bool test = true;

    while(test) {

        const char input = getch();

        switch(input) {

            case 'z': {
                x += pas;
                break;
            }

            case 's': {
                x -= pas;
                break;
            }

            case 'd': {
                y -= pas;
                break;
            }

            case 'q': {
                y += pas;
                break;
            }

            case 'r': {
                z += pas;
                break;
            }

            case 'f': {
                z -= pas;
                break;
            }

            case 'a': {
                pas++;
                if(pas > 10) {pas = 10;}
                break;
            }

            case 'e': {
                pas--;
                if(pas < 1) {pas = 1;}
                break;
            }

            case ' ': {
                //save pos
                std::cout << "Pos saved : " << x << " " << y << " " << z << std::endl;
                Movement mov;
                mov.setMode(MovementMode::COORDINATES, false);
                mov.setCoordinates(x, y, z);
                mov.setDuration(1000);
                move.push_back(mov);
                break;
            }

            case 10:{test = false;}
            case 27:{test = false;}

            default:{std::cout << "unknown key : " << input << " " << static_cast<int>(input) << std::endl;}

        }
        std::cout << std::endl << "Pas = " << pas << std::endl;
        PlaceArm(x, y, z);
        MoveArm(true);
        std::cout << std::endl;
    }
    std::string path = "CSV/seq_"+std::to_string(caractere);
    SequenceWriter seq(move,path);
    seq.generate(Format::CSV, true);
}

void Arm::WriteOn() {SetAxePosTic(6, m_LimMinArm[5]);}
void Arm::WriteOff() {SetAxePosTic(6, m_LimMaxArm[5]);}
void Arm::Homing() {
    SetAxePos(1, 0);
    SetAxePos(2, 0);
    SetAxePos(3, 0);
    SetAxePos(4, 0);
    SetAxePos(5, 0);
    WriteOff();
    MoveArm(true);
}

void Arm::PosToMove() {
    SetAxePos(1, 0);
    SetAxePos(2, M_PI/2);
    SetAxePos(3, -M_PI/2);
    SetAxePos(4, 0);
    SetAxePos(5, M_PI/2);
    WriteOff();
}


void Arm::SetTime(const int time) {

    m_TimeArm = time;

    if(time < limTimeMin) {
        m_TimeArm = limTimeMin;
        std::cout << "time arm sat min" << std::endl;
    }
    if(time > limTimeMax) {
        m_TimeArm = limTimeMax;
        std::cout << "time arm sat max" << std::endl;
    }

}
void Arm::SetLimMinAxe(int nb, int lim) {m_LimMinArm[nb-1] = lim;}
void Arm::SetLimMaxAxe(int nb, int lim) {m_LimMaxArm[nb-1] = lim;}
void Arm::SetAxePos(int nb, double pos) {
    if(pos < -M_PI/2) {pos = -M_PI/2;}
    if(pos > M_PI/2) {pos = M_PI/2;}
    int posi = static_cast<int>((((pos+M_PI/2)*(m_LimMaxArm[nb-1]-m_LimMinArm[nb-1]))/M_PI)+m_LimMinArm[nb-1]);
    SetAxePosTic(nb, posi);
}
void Arm::SetAxePosTic(int nb, int pos) {
    if(pos < m_LimMinArm[nb-1]) {pos = m_LimMinArm[nb-1];}
    if(pos > m_LimMaxArm[nb-1]) {pos = m_LimMaxArm[nb-1];}
    m_PosArm[nb-1] = pos;
}
void Arm::SetLimAxe(int nb, int lim_min, int lim_max) {
    m_LimMinArm[nb-1] = lim_min;
    m_LimMaxArm[nb-1] = lim_max;
}

int Arm::GetBdRate(void) {return m_usb->GetBdRate();}
int Arm::GetPortNb(void) {return m_usb->GetPortNb();}
int Arm::GetTime(void) {return m_TimeArm;}
int Arm::GetNbMot(void) {return m_nb;}
int Arm::GetLimMinAxe(int nb) {return m_LimMinArm[nb-1];}
int Arm::GetLimMaxAxe(int nb) {return m_LimMaxArm[nb-1];}
int Arm::GetAxePos(int nb) {return m_PosArm[nb-1];}

