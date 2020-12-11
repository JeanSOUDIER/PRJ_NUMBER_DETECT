#ifndef SEQUENCER_H
#define SEQUENCER_H

#include <iostream>
#include <string.h>
#include <valarray>
#include <ctime>
#include <chrono>
#include <vector>

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

/**

                     Bluetooth class

    DUHAMEL Erwan (erwanduhamel@outlook.com)
    SOUDIER Jean  (jean.soudier@insa-strasbourg.fr)


    Provides a class for Bluetooth communication.

    Features :

    • Send and read data from bluetooth

    Functions :

    • Sequencer                     | Constructor
    • Execute()                     | Function to do the main programme 1 turn
    • MoveRobot(time)               | Function to move the mobile base with feedback loop for a time
    • RollOver(nb)             		| Function to finish the turn (nb the number of caracters with is actully write)

*/

class Sequencer {
	public:
		explicit Sequencer(Arm* = nullptr, MobileBase* = nullptr);
		explicit Sequencer(Arm* = nullptr, Bluetooth* = nullptr, MobileBase* = nullptr);
		~Sequencer();

        bool Execute();
    protected:
    	void MoveRobot(const uint64_t time);
	void RollOver(int nb);

    	bool m_BLE_start;

	private:
		double m_coordinatesA;
		double m_coordinatesX;
		double m_coordinatesY;

		Arm* m_WidowXL;
		Bluetooth* m_BLE;
		MobileBase* m_TurtleBot;

		Control::System_project* m_sys;

		SequenceHandler seqHandler;
};

#endif //SEQUENCER_H
