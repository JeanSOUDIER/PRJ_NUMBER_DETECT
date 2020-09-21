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
}

Sequencer::~Sequencer() {
	delete m_WidowXL;
	delete m_BLE;
	delete m_TurtleBot;
}

void Sequencer::Execute() {
	while(1) {
		std::vector<unsigned char> reading = m_BLE->Read();
		if(reading[0] == 252) {break;}
		m_TurtleBot->Go(0,0);
		int j = 1, arm_dist;
		for(int i=0;i<static_cast<unsigned char>(reading.size());i++) {
			if(i%MAX_LENGTH_ARM >= j) {
				j++;
				m_TurtleBot->Go(DIST_BASE*j,0);
			}
			std::vector<Movement*> Move = seqHandler.find(reading[i]).getMovements_STD();
			for(int k=0;k<Move.size();k++) {
				arm_dist = DIST_ARM*(i%MAX_LENGTH_ARM-MAX_LENGTH_ARM/2);
				m_WidowXL->PlaceArm(Move[k].getX()+arm_dist, m_TurtleBot.getDistBoard(), Move[k].getZ());
				m_WidowXL->SetTime(Move.getDuration());
				m_WidowXL->MoveArm(true);
			}
		}
		m_BLE->WriteEnd();
	}
}
