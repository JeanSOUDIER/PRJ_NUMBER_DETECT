#ifndef SEQUENCER_H
#define SEQUENCER_H

#include "Arm.hpp"
#include "Lidar.hpp"
#include "Bluetooth.hpp"
#include "MobileBase.hpp"
//#include "../SequenceHandler/sequencehandler.h"
//#include "../SequenceHandler/release/sequencehandler.o"

constexpr int DIST_BASE = 200;
constexpr int DIST_ARM = 50;
constexpr int MAX_LENGTH_ARM = DIST_BASE/DIST_ARM;

class Sequencer {
	public:
		explicit Sequencer(Arm* = nullptr, Bluetooth* = nullptr, MobileBase* = nullptr);
		~Sequencer();

		void Execute();
	private:
		Arm* m_WidowXL;
		Bluetooth* m_BLE;
		MobileBase* m_TurtleBot;
};

#endif //SEQUENCER_H