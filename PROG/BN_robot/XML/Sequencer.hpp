#ifndef SEQUENCER_H
#define SEQUENCER_H

#include <iostream>
#include <string.h>

#include "Arm.hpp"
#include "Lidar.hpp"
#include "Bluetooth.hpp"
#include "MobileBase.hpp"
#include "sequencehandler.h"

constexpr int DIST_BASE = 50;
constexpr int DIST_ARM = 50;
constexpr int MAX_LENGTH_ARM = DIST_BASE/DIST_ARM;

class Sequencer {
	public:
		explicit Sequencer(Arm* = nullptr, MobileBase* = nullptr);
		explicit Sequencer(Arm* = nullptr, Bluetooth* = nullptr, MobileBase* = nullptr);
		~Sequencer();

        bool Execute();
    protected:
    	bool m_BLE_start;

	private:
		Arm* m_WidowXL;
		Bluetooth* m_BLE;
		MobileBase* m_TurtleBot;

		SequenceHandler seqHandler;
};

#endif //SEQUENCER_H
