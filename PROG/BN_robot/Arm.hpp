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
#include "sequencehandler.h"
#include "sequencewriter.h"

constexpr int ARB_SIZE_POSE = 7;
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
                std::vector<int> PosWriting(bool state);
                void PosToMove();

                void ToKeyboard();
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
