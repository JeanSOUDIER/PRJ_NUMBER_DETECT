#ifndef SEQUENCER_H
#define SEQUENCER_H

#include <iostream>
#include <string.h>
#include <valarray>
#include <ctime>
#include <chrono>

#include "Arm.hpp"
#include "Lidar.hpp"
#include "Bluetooth.hpp"
#include "MobileBase.hpp"
#include "../SequenceHandler_STL/sequencehandler.h"
#include "../Control/system_project.h"
#include "../Control/Control_global.h"

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
    	void MoveRobot(const uint64_t time);

    	bool m_BLE_start;

	private:
		Arm* m_WidowXL;
		Bluetooth* m_BLE;
		MobileBase* m_TurtleBot;

		Control::System_project* m_sys;

		SequenceHandler seqHandler;
};

#endif //SEQUENCER_H
