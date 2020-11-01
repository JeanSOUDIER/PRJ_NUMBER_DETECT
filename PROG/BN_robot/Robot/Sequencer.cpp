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
	int length;
	std::vector<char> reading = {static_cast<std::vector<char>>(255)};
	if(m_BLE_start) {
		while(reading.at(0) == 255) {
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
		std::cout << reading.at(i) << std::endl;
	}
	//m_TurtleBot->GoPos(0,0,0);
	m_TurtleBot->SetSpeedCons(0);
	m_WidowXL->PosWriting(true, 2000);
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
			//m_TurtleBot->GoPos(DIST_BASE,0,0); //*(i+1)
			m_TurtleBot->SetSpeedCons(100);
		} else {
			m_WidowXL->MoveArm(true);
		}
	}
	if(m_BLE_start) {m_BLE->SetTX("done");}
	return true;
}
