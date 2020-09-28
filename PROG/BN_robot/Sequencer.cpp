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
	std::string path;
	for(int i=0;i<256;i++) {
		path = "CSV/seq_"+std::to_string(i)+".csv";
		seqHandler.addSequence(static_cast<unsigned char>(i),path);
	}
    std::cout << "Sequencer start" << std::endl;
}

Sequencer::~Sequencer() {
	delete m_WidowXL;
	if(m_BLE_start) {delete m_BLE;}
	delete m_TurtleBot;
}

bool Sequencer::Execute() {
	unsigned char length = 1, re = 0;
	if(m_BLE_start) {
		while(m_BLE->Read() != 255) {}
		length = m_BLE->Read();
		re = m_BLE->Read();
	} else {
		std::cout << "length = ";
		std::cin >> length;
		length -= '0'-1;
		std::cout << static_cast<int>(length) << std::endl;
	}
	std::vector<unsigned char> reading(length);
	if(re == 252) {return false;} else {reading.at(0) = re;}
	for(unsigned int i=1;i<length;i++) {
		if(m_BLE_start) {
			reading.at(i) = m_BLE->Read();
		} else {
			std::cout << i << " : ";
			std::cin >> reading.at(i);
		}
	}
	//m_TurtleBot->GoPos(0,0,0);
	for(unsigned int i=0;i<reading.size();i++) {
		std::vector<Movement> Move = seqHandler.find(reading[i]).getMovements();
		m_WidowXL->WriteOff();
		for(unsigned int k=0;k<Move.size();k++) {
			m_WidowXL->PlaceArm(Move[k].getX(), Move[k].getY(), Move[k].getZ());
			m_WidowXL->SetTime(Move[k].getDuration());
			m_WidowXL->MoveArm(true);
			m_WidowXL->WriteOn();
		}
		m_WidowXL->SetTime(2000);
		m_WidowXL->WriteOff();
        m_WidowXL->MoveArm(true);
		m_WidowXL->PosToMove();
        m_WidowXL->MoveArm(true);
		if(i < reading.size()-1) {
			m_TurtleBot->GoPos(DIST_BASE,0,0);
		}
	}
	if(m_BLE_start) {m_BLE->WriteEnd();}
	return true;
}
