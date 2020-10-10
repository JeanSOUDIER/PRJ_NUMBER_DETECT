#ifndef LIDAR_H
#define LIDAR_H

#include <iostream>
#include <wiringPi.h>
#include <cmath>
#include <vector>
#include <array>
#include <pthread.h>
#include <atomic>
#include <string.h>
#include "Usb.hpp"
#include "algorithm"
#include "utility.h"

class MobileBase;

class Lidar{
        friend class MobileBase;

	public:
        Lidar(const int nb_usb, const int bdrate);
        Lidar(const bool start, const int nb_usb, const int bdrate);
        ~Lidar();

        void SetStart(const bool state);
        void SetSat(bool state);
        void Poll(void);

        int GetBdRate(void);
        int GetPortNb(void);
        bool GetStart(void);
        int GetMotorSpeed(void);
        int GetTimeIncrement(void);
        bool GetSat();
        std::vector<int> GetRange(void);
        std::vector<int> GetIntensity(void);
        std::vector<double> GetXPos(void);
        std::vector<double> GetYPos(void);

        void Display(const bool isXY);
        void DisplayGraph();
        bool SaveLidarPoints();
        static void* LidarHelper(void *context);
        void StartThread();
protected:
        void* ThreadRun();

private:
        void StartLidar(void);

        const int TERMINAL_LENGTH = 127;
        const double TERMINAL_STEP = TERMINAL_LENGTH/3500;

        std::atomic<bool> m_start;

        int m_port_nr;
        int m_bdrate;

        std::array<std::atomic<int> , 360> m_range;
        std::array<std::atomic<int> , 360> m_intensity;
        std::array<std::atomic<double> , 360> m_xPos;
        std::array<std::atomic<double> , 360> m_yPos;

        bool m_sat = true;

        int m_motor_speed = 0;
        int m_time_increment;
        uint16_t rpms;

        Usb *m_usb;
        pthread_t *inc_x_thread;
};
#endif //LIDAR_H
