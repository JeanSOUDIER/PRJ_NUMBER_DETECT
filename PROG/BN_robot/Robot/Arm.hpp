#ifndef ARM_H
#define ARM_H

#include <iostream>
#include <complex>
#include <cmath>
#include <vector>
#include <wiringPi.h>
#include <termios.h>
#include <string.h>
#include "Usb.hpp"
#include "../Joystick/js.hpp"
#include "../SequenceHandler_STL/sequencehandler.h"
#include "../SequenceHandler_STL/sequencewriter.h"

/**

                     Arm class

    DUHAMEL Erwan (erwanduhamel@outlook.com)
    SOUDIER Jean  (jean.soudier@insa-strasbourg.fr)


    Provides a class for Arm control.

    Features :

    • Full position control of a 6 axis arm

    Functions :

    • Arm                           | Constructor with arguments nb (number of motors), nb_usb (the port number of the arm in USB), the baudrate of the USB, the limits (min/max), the time to move the arm
    • SetLimAxe(nb,lim_min,lim_max) | Function that manage the position limits of a motor with nb the number of the motor, lim_min the minimum value and lim_max the max value
    • SetLimMinAxe(nb,lim)          | Function to manage the min limit of the motor nb
    • SetLimMaxAxe(nb,lim)          | Function to manage the max limit of the motor nb
    • SetAxePos(nb,pos)             | Function to change the goal position to a motor to a position between pi/2 and -pi/2
    • SetAxePosTic(nb,pos)          | Function to change the goal position to a motor to a position between lim_max and lim_min
    • GetAxePos(nb)                 | Function that return the psition of the motor in Tic
    • GetBdRate()                   | Function that return the baudrate
    • GetPortNb()                   | Function that return the USB port number
    • GetTime()                     | Function that return the time to move motors
    • GetNbMot()                    | Function that return the number of motors
    • MoveArm(withDelay)            | Function to move the arm with the positions in the buffer [SetAxePos or SetAxePosTic]
    • PlaceArm(x,y,z)               | Function to move the arm with inverse kinematic to an (x,y,z) position of the end effector from the base of the arm, return the success
    • WriteOn()                     | Function that set the position of the end effector to write
    • WriteOff()                    | Function that set the position of the end effector to not write
    • Homing()                      | Function to place the robot to the start position
    • PosWriting(state,time)        | Function to place the arm to write something with state of the end effector (on/off) and the time to go
    • PosPreWriting(state,time)     | Function to pre place the arm to write something with state of the end effector (on/off) and the time to go
    • PosToMove()                   | Function to place the robot to a good position to move the mobile base
    • ToKeyboard(GamePad)           | Function to move the arm with a control of a Keyboard (GamePad=false) or a GamePad Xbox360 (GamePad=true)
    • Send(ins,data)                | Function to send a command to the arm with ins the instruction and data the datas
    • getch()                       | Function to get the actual key with is pess on the keyboard

    Const variables :

    • Lr                            | Variable of the min length allow to the arm in coordinate r = Norm(X,Y)
    • Lz                            | Variable of the min Z allowed


                         A4b
                Motor6---------End effector
                   | 
                   | A4a
                   |
                Motor5


                -------Motor3--------------------------Motor4
                   |                      A2              \
                   |                                       \
                    |                                       \ A3
                    |                                        \
                     |                                        \
                     | A1                                   Motor5-------Motor6
                      |                                             A4b
                      |  |
        Z              |-| A1thet
        /\             | |
        |               ||
        |            Motor2
        _______________________________
        |            Motor1           |
    _______________________________________----> R = Norm(X,Y)

*/

constexpr int ARB_SIZE_POSE = 7; //instructions
constexpr int ARB_LOAD_POSE = 8;
constexpr int ARB_LOAD_SEQ  = 9;
constexpr int ARB_PLAY_SEQ  = 10;
constexpr int ARB_LOOP_SEQ  = 11;
constexpr int ARB_DO_FULL   = 12;

class Arm {
        public:
                Arm(const int nb,const int nb_usb, const int bdrate);
                Arm(const int nb, const int nb_usb, const int bdrate, const int time);
                Arm(const int nb, const int nb_usb, const int bdrate, const std::vector<int> &lim_min, const std::vector<int> &lim_max);
                Arm(const int nb, const int nb_usb, const int bdrate, const std::vector<int> &lim_min, const std::vector<int> &lim_max, int time);
                ~Arm();

                void SetLimAxe(int nb, int lim_min, int lim_max);
                void SetLimMinAxe(int nb, int lim);
                void SetLimMaxAxe(int nb, int lim);
                void SetAxePos(int nb, double pos);
                void SetAxePosTic(int nb, int pos);
                void SetTime(const int time);

                int GetLimMinAxe(int nb);
                int GetLimMaxAxe(int nb);
                int GetAxePos(int nb);
                int GetBdRate(void);
                int GetPortNb(void);
                int GetTime(void);
                int GetNbMot(void);

                void MoveArm(bool withDelay);
                bool PlaceArm(double x, double y, double z);

                void WriteOn();
                void WriteOff();
                void Homing();
                std::vector<int> PosWriting(bool state, int time);
                std::vector<int> PosPreWriting(bool state, int time);
                void PosToMove();

                void ToKeyboard(bool GamePad);
        private:
                void Send(int ins, const std::vector<char> &data);
                char getch();

                const int Lr = 179;
                const int Lz = -236;
                const int A1 = 155;
                const int A2 = 150;
                const int A3 = 80;
                const int A4a = 110;
                const int A4b = 105;
                const int A4 = sqrt(A4a*A4a+A4b*A4b);
                const double A1thet = 0.37;


                const int limTimeMin = 300;
                const int limTimeMax = 60000;

                std::vector<int> m_PosArm;
                std::vector<int> m_LimMinArm;
                std::vector<int> m_LimMaxArm;

                bool m_active = false;
                int m_port_nr;
                int m_bdrate; 
                int m_TimeArm = 5000;
                int m_nb = 6;

                Usb *m_usb;
};

#endif //ARM_H
