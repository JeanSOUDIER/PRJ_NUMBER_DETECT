#include "Sequencer.hpp"

Sequencer::Sequencer(Arm* WidowXL, MobileBase* TurtleBot)
    : Sequencer(WidowXL,nullptr,TurtleBot) {}

Sequencer::Sequencer(Arm* WidowXL, Bluetooth* BLE, MobileBase* TurtleBot) {
    m_WidowXL = WidowXL;
    if(BLE) {
        m_BLE_start = true;
        m_BLE = BLE;
    } else {
        m_BLE_start = false;
    }
    m_TurtleBot = TurtleBot;
    m_TurtleBot->SetSpeed(0, 0);
    std::string path;
    for(int i=0;i<256;i++) {
        path = "CSV/seq_"+std::to_string(i)+".csv";
        seqHandler.addSequence(static_cast<unsigned char>(i),path);
    }
    m_sys = new Control::System_project{20, m_TurtleBot, {0, 0, -M_PI/2}, 10, 5};
    m_coordinatesA = m_TurtleBot->GetAngleStart();
    m_coordinatesY = m_TurtleBot->GetPosStart(false,(m_coordinatesA*M_PI/180),700);
    m_coordinatesX = m_TurtleBot->GetPosStart(false,std::fmod((m_coordinatesA*M_PI/180)+271,359),5000);
    std::cout << "coord init " << m_coordinatesA << " " << m_coordinatesX << " " << m_coordinatesY << std::endl;
    std::cout << "Sequencer start" << std::endl;
}

Sequencer::~Sequencer() {
    delay(1000);
    m_TurtleBot->SetSpeed(0, 0);
    delete m_WidowXL;
    if(m_BLE_start) {delete m_BLE;}
    delete m_sys;
    delete m_TurtleBot;
}

bool Sequencer::Execute() {
    int length;
    std::vector<char> reading = {0};
    if(m_BLE_start) {
        while(reading.at(0) == 0) {
            reading = m_BLE->GetRX();
        }
        if(reading.at(0) == 254) {return false;}
    } else {
        std::cout << "length = ";
        int l;
        std::cin >> l;
        if(l < 1) {return false;}
        length = static_cast<unsigned char>(l);
        length++;
        std::cout << static_cast<unsigned char>(length) << std::endl;
        int inp;
        reading.clear();
        for(int i=1;i<length;i++) {
            std::cout << i << " : ";
            std::cin >> inp;
            reading.push_back(static_cast<unsigned char>(inp)+'0');
        }
    }
    for(unsigned int i=0;i<reading.size();i++) {
        std::cout << static_cast<int>(reading.at(i)) << " ";
    }
    std::cout << std::endl;
    unsigned int tempo;
    m_WidowXL->PosPreWriting(false, 4000);
    m_WidowXL->PosWriting(true, 1000);
    for(unsigned int i=0;i<reading.size();i++) {
        std::vector<Movement> Move = seqHandler.find(reading[i]).getMovements();
        m_WidowXL->SetTime(0);
        for(unsigned int k=0;k<Move.size();k++) {
            m_WidowXL->PlaceArm(Move[k].getX(), Move[k].getY(), Move[k].getZ());
            m_WidowXL->MoveArm(true);
            if(k == 0) {
                m_WidowXL->WriteOn();
                m_WidowXL->MoveArm(true);
            }
        }
        m_WidowXL->WriteOff();
        m_WidowXL->MoveArm(true);
        m_WidowXL->PosWriting(false, 0);
        if(i < reading.size()-1) {
            m_WidowXL->MoveArm(false);
            delay(20);
            if(i == 10) {
                RollOver(10);
            } else {
                MoveRobot(1000);
            }
        } else {
            m_WidowXL->MoveArm(true);
        }
        tempo = i;
    }
    RollOver(tempo);
    if(m_BLE_start) {m_BLE->SetTX("done");}
    double coordinatesA = m_coordinatesA-m_TurtleBot->GetAngleStart();
    double coordinatesY = m_coordinatesY-m_TurtleBot->GetPosStart(false,(coordinatesA*M_PI/180),700);
    double coordinatesX = m_coordinatesX-m_TurtleBot->GetPosStart(false,std::fmod((m_coordinatesA*M_PI/180)+271,359),5000);
    std::cout << "coord buf " << coordinatesA << " " << coordinatesX << " " << coordinatesY << std::endl;
    m_TurtleBot->SetCurrentPosi({coordinatesA, coordinatesX, coordinatesY});
    return true;
}

void Sequencer::MoveRobot(const uint64_t time) {
    auto begin_timestamp = std::chrono::high_resolution_clock::now();
    auto begining_timestamp = std::chrono::high_resolution_clock::now();
    auto current_timestamp = std::chrono::high_resolution_clock::now();
    m_TurtleBot->SetSpeedCons(1);
    do {
        current_timestamp = std::chrono::high_resolution_clock::now();
        if(std::chrono::duration_cast<std::chrono::milliseconds>(current_timestamp - begin_timestamp).count() >= m_sys->Ts()) {
            begin_timestamp = std::chrono::high_resolution_clock::now();
            m_sys->compute();
            m_TurtleBot->SetCurrentPosi(m_sys->coord());
            m_TurtleBot->SetSpeed(m_sys->vr(), m_sys->vl());
            std::cout << "t " << std::chrono::duration_cast<std::chrono::milliseconds>(current_timestamp - begin_timestamp).count() << std::endl << std::endl;
        }
    } while(std::chrono::duration_cast<std::chrono::milliseconds>(current_timestamp - begining_timestamp).count() < time);
    m_TurtleBot->SetSpeedCons(0);
    delay(m_sys->Ts());
    m_TurtleBot->SetSpeed(0, 0);
    delay(m_sys->Ts());
    m_TurtleBot->SetSpeed(0, 0);
}

void Sequencer::RollOver(int nb) {
    m_WidowXL->SetTime(4000);
    m_WidowXL->PosToMove();
    m_WidowXL->MoveArm(true);
    MoveRobot(21000+1000*(10-nb));
}
