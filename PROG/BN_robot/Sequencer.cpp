#include "Sequencer.hpp"

Sequencer::Sequencer(Arm* WidowXL, Bluetooth* BLE, MobileBase* TurtleBot) {
	m_WidowXL = WidowXL;
	m_BLE = BLE;
	m_TurtleBot = TurtleBot;
	std::string path;
	/*for(int i=0;i<256;i++) {
		path = "/XML/seq_"+std::to_string(i)+".xml";
		seqHandler.addSequence(static_cast<unsigned char>(i),path);
	}*/
}

Sequencer::~Sequencer() {
	delete m_WidowXL;
	delete m_BLE;
	delete m_TurtleBot;
}

int Sequencer::Execute() {
	int result = 0;
	std::vector<unsigned char> reading = m_BLE->Read();
	if(reading[0] == 252) {result = 1;}
	m_TurtleBot->Go(0,0);
	for(int i=0;i<static_cast<unsigned char>(reading.size());i++) {
		m_TurtleBot->Go(DIST_BASE,0);
		/*std::vector<Movement*> Move = seqHandler.find(reading[i]).getMovements_STD();
		for(int k=0;k<Move.size();k++) {
			m_WidowXL->PlaceArm(Move[k].getX(), Move[k].getY(), Move[k].getZ());
			m_WidowXL->SetTime(Move.getDuration());
			m_WidowXL->MoveArm(true);
		}*/
	}
	m_BLE->WriteEnd();
	return result;
}
