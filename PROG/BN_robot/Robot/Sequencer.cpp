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
	//std::valarray<Control::scalar> posi = static_cast<std::valarray<Control::scalar>>(m_TurtleBot->GetCurrentPos());
	//m_sys = new Control::System_project{20, m_TurtleBot, {posi[0], posi[1]}, 1, 0.5};
	m_syst = new asserv(20, 2000, 1000);
	std::cout << "Sequencer start" << std::endl;
}

Sequencer::~Sequencer() {
	delay(1000);
	m_TurtleBot->SetSpeed(0, 0);
	delete m_WidowXL;
	if(m_BLE_start) {delete m_BLE;}
	//delete m_sys;
	delete m_syst;
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
			MoveRobot(2500);
		} else {
			m_WidowXL->MoveArm(true);
		}
	}
	if(m_BLE_start) {m_BLE->SetTX("done");}
	return true;
}

void Sequencer::MoveRobot(const uint64_t time) {
    auto begin_timestamp = std::chrono::high_resolution_clock::now();
    auto begining_timestamp = std::chrono::high_resolution_clock::now();
    auto current_timestamp = std::chrono::high_resolution_clock::now();
    //m_TurtleBot->SetSpeedCons(0.1);
    m_syst->SetSpeedNorm(100);
    int v = -50;
    do {
        current_timestamp = std::chrono::high_resolution_clock::now();
        if(std::chrono::duration_cast<std::chrono::milliseconds>(current_timestamp - begin_timestamp).count() >= m_syst->GetTe()) {
            /*m_sys->compute();
	    double x = m_sys->vx();
	    double y = m_sys->vy();
	    std::cout << "vx " << x << " vy " << y << std::endl;
            m_TurtleBot->SetSpeed(y, x);*/
	    //std::vector<double> test = m_TurtleBot->GetCurrentPos();
	    std::vector<int> teste = m_syst->Compute({0, 0, 0});
	    //v *= -1;
	    //m_TurtleBot->SetSpeed(v,v);
	    m_TurtleBot->SetSpeed(teste);
	    std::cout << "t " << std::chrono::duration_cast<std::chrono::milliseconds>(current_timestamp - begin_timestamp).count() << std::endl << std::endl;
            begin_timestamp = std::chrono::high_resolution_clock::now();
        }
    } while(std::chrono::duration_cast<std::chrono::milliseconds>(current_timestamp - begining_timestamp).count() < time);
    //m_TurtleBot->SetSpeedCons(0);
    m_syst->SetSpeedNorm(0);
    delay(m_syst->GetTe());
    m_TurtleBot->SetSpeed(0, 0);
    delay(m_syst->GetTe());
    m_TurtleBot->SetSpeed(0, 0);
    delay(m_syst->GetTe());
    m_TurtleBot->SetSpeed(0, 0);
    delay(m_syst->GetTe());
    m_TurtleBot->SetSpeed(0, 0);
}
