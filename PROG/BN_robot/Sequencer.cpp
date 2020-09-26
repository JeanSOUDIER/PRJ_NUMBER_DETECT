#include "Sequencer.hpp"

Sequencer::Sequencer(Arm* WidowXL, Bluetooth* BLE, MobileBase* TurtleBot) {
	m_WidowXL = WidowXL;
	m_BLE = BLE;
	m_TurtleBot = TurtleBot;
	std::string path;
	for(int i=0;i<256;i++) {
		path = "/XML/seq_"+std::to_string(i)+".xml";
		seqHandler.addSequence(static_cast<unsigned char>(i),path);
	}
    std::cout << "Sequencer start" << std::endl;
}

Sequencer::~Sequencer() {
	delete m_WidowXL;
	delete m_BLE;
	delete m_TurtleBot;
}

bool Sequencer::Execute() {
	while(m_BLE->Read() != 255) {}
	unsigned char length = m_BLE->Read();
	std::vector<unsigned char> reading(length);
	unsigned char re = m_BLE->Read();
	if(re == 252) {return false;} else {reading.at(0) = re;}
	for(int i=1;i<length;i++) {
		reading.at(i) = m_BLE->Read();
	}
	m_TurtleBot->Go(0,0);
	for(int i=0;i<static_cast<unsigned char>(reading.size());i++) {
		m_TurtleBot->Go(DIST_BASE,0);
		//std::vector<Movement*> Move = seqHandler.find(reading[i]);
		/*for(int k=0;k<Move.size();k++) {
			m_WidowXL->PlaceArm(Move[k].getX(), Move[k].getY(), Move[k].getZ());
			m_WidowXL->SetTime(Move.getDuration());
			m_WidowXL->MoveArm(true);
		}*/
	}
	m_BLE->WriteEnd();
	return true;
}
