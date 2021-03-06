#include "Arm.hpp"

Arm::Arm(const int nb, const int nb_usb, const int bdrate)
    : Arm(nb,nb_usb,bdrate,{1750, 2000, 500, 500, 200, 300},{3850, 4100, 2600, 2600, 820, 700},5000){}

Arm::Arm(const int nb, const int nb_usb,const int bdrate, const int time)
    : Arm(nb,nb_usb,bdrate,{1750, 2000, 500, 500, 200, 300},{3850, 4100, 2600, 2600, 820, 700},time) {}

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
        delay(10000); //Do not remove, USB won't start correctly and you will not be able to control the arm
        PosToMove();
        MoveArm(true);
        PosToMove();
        MoveArm(true);
    }

    SetTime(time);
    std::cout << "Arm start" << std::endl;
}

Arm::~Arm() {
    SetTime(5000);
    PosToMove();
    MoveArm(true);
    delete m_usb;
}


void Arm::MoveArm(bool withDelay) {
    if(m_usb->GetActive()) {
        std::vector<char> sending(2*m_nb+2); //format <Time(8L bits), Time(8H bits), Pos[i](8L bits), Pos[i](8H bits)....>
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
    int sum = 508+data.size()+2+ins; //compute checksum
    std::vector<char>send(5+data.size());
    send[0] = char(255); //start char
    send[1] = char(253); //id of arbotix
    send[2] = char(data.size()+2); //size msg
    send[3] = char(ins); //instruction
    for(unsigned int i=0;i<data.size();i++) {
        send[i+4] = data[i]; //add datas
        sum += data[i]; //compute checksum
    }
    send[data.size()+4] = char(255-((sum%256)+1)); //compute checksum
    m_usb->SendBytes(send); //sending
}

bool Arm::PlaceArm(double x, double y, double z) {
    std::cout << "x = " << x << ", y = " << y << ", z = " << z << std::endl;
    bool test = false;
    x -= A4b; //map to positives values
    y -= A4a;
    z *= -1;
    //convet to cartesian
    const double r = sqrt(x*x+y*y); //compute r
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
                const double d = std::sqrt(rA3*rA3+z*z);
                if(d < A1+A2) {
                    const double thet = std::acos(z/d); //use Al-kashi formula to find the angle of Motor2 from bottom to A1
                    const double thetLoi = std::acos(-(A1*A1+d*d-A2*A2)/(2*d*A1)); //use the generic theorem of pythagor to find the angle of Motor2 from Motor2 to normal of the bottom
                    const double theta1 = -thetLoi+thet+A1thet; //angle of the bottom to A1 + angle of the structure A1thet - angle of the normal to the Motor2

                    const double thetLoi2 = std::acos(-(A1*A1+A2*A2-d*d)/(2*A2*A1)); //use the generic theorem of pythagor to find the angle of the Motor3
                    const double theta2 = M_PI/2-thetLoi2; //adjust the value

                    const double theta3 = -theta1-theta2; //force the 3rd angle to conserve the A3 parralella to the bottom
                    //set axes
                    SetAxePos(1, gamma);
                    SetAxePos(2, theta1);
                    SetAxePos(3, theta2);
                    SetAxePos(4, theta3);
                    SetAxePos(5, M_PI/2-gamma);
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

void Arm::ToKeyboard(bool GamePad) {
    std::vector<int> pos = PosWriting(true, 5000);

    int x = pos[0];
    int y = pos[1];
    int z = pos[2];
    int pas = 10;
    SetTime(0);
    std::cout << "Entrez votre caractere : ";
    char caractere;
    std::cin >> caractere;
    std::cout << "Votre caractere est : " << caractere << std::endl;
    std::vector<Movement> move;
    bool test = true;

    Js Xbox360(GamePad);
    int bt_state = 1;

    while(test) {
        if(GamePad) {
            std::vector<int> res = Xbox360.GetEvent();
            if(res[0] == static_cast<int>(GamePadType::Button)) {
                if(res[2]) {
                    switch(res[1]) {
                        case 0:{
                            //save pos
                            std::cout << "Pos saved : " << x << " " << y << " " << z << std::endl;
                            Movement mov;
                            mov.setMode(MovementMode::COORDINATES, false);
                            mov.setCoordinates(x, y, z);
                            mov.setDuration(1000);
                            move.push_back(mov);
                            break;}
                        case 1:{
                            bt_state = 1;
                            break;}
                        case 2:{
                            bt_state = 2;
                            break;}
                        case 3:{
                            bt_state = 3;
                            break;}
                        case 4:{
                            WriteOn();
                            break;}
                        case 5:{
                            WriteOff();
                            break;}
                        case 7:{
                            test = false;
                            break;}
                        case 6:{
                            std::cout << "no save" << std::endl;
                            return;}
                        default:{
                            std::cout << "unknow key " << res[1] << std::endl;
                            break;}
                    }
                }
            } else if(res[0] == static_cast<int>(GamePadType::Axis)) {
                if(bt_state == 1) {
                    x += res[3];
                } else if(bt_state == 2) {
                    y += res[3];
                } else {
                    z += res[3];
                }
            }
        } else {
            const char input = getch();

            switch(input) {

                case 'z': {
                    pas = 1;
                    x += pas;
                    break;
                }

                case 's': {
                    pas = 1;
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
                    pas += 10;;
                    if(pas > 50) {pas = 50;}
                    break;
                }

                case 'e': {
                    pas -= 10;
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
                    pas = 10;
                    break;
                }

                case 'o': {
                    test = false;
                    break;
                }

                case 126:{
                    std::cout << "no save" << std::endl;
                    return;
                }

                default:{std::cout << "unknown key : " << input << " " << static_cast<int>(input) << std::endl;}

            }
            std::cout << std::endl << "Pas = " << pas << std::endl;
        }
        PlaceArm(x, y, z);
        MoveArm(true);
        std::cout << std::endl;
    }
    std::cout << "Saving" << std::endl;
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
std::vector<int> Arm::PosWriting(bool state, int time) {
    std::vector<int> pos(3);
    pos[0] = 300;
    pos[1] = 220;
    pos[2] = 0;

    SetTime(time);
    PlaceArm(pos[0], pos[1], pos[2]);
    if(state) {
        WriteOn();
    } else {
        WriteOff();
    }
    MoveArm(true);
    return pos;
}
std::vector<int> Arm::PosPreWriting(bool state, int time) {
    std::vector<int> pos(3);
    pos[0] = 280;
    pos[1] = 200;
    pos[2] = 0;

    SetTime(time);
    PlaceArm(pos[0], pos[1], pos[2]);
    if(state) {
        WriteOn();
    } else {
        WriteOff();
    }
    MoveArm(true);
    return pos;
}
void Arm::PosToMove() {
    SetAxePos(1, -M_PI/2);
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

