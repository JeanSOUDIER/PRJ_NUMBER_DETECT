#ifndef LIDAR_H
#define LIDAR_H

#include <iostream>
#include <wiringPi.h>
#include <cmath>
#include <vector>
#include <array>
#include <pthread.h>
#include <atomic>
#include <limits>
#include <string.h>
#include "Usb.hpp"
#include "algorithm"
#include "../SequenceHandler_STL/utility.h"

/**

                     Lidar class

    DUHAMEL Erwan (erwanduhamel@outlook.com)
    SOUDIER Jean  (jean.soudier@insa-strasbourg.fr)


    Provides a class for Lidar control.

    Features :

    • save lidar points

    Functions :

    • Lidar                         | Constructor with arguments nb_usb (the number of the USB port), bdrate (the baudrate) and start (if you want to start the lidar)
    • SetStart(state)               | Function that starts or stops the lidar
    • Poll()                        | Function to read lidar points
    • GetBdRate()                   | Function that returns the baudrate
    • GetPortNb()                   | Function that returns the port number
    • GetStart()                    | Function that returns the state of the lidar
    • GetMotorSpeed()               | Function that returns the motor speed
    • GetTimeIncrement()            | Function that returns the time increment between frames
    • GetSat()                      | Function that returns if we data from 0 to 150 mm is saturated to "std::inf"
    • GetRange()                    | Function that returns the range of the last frame
    • GetIntensity()                | Function that returns the intensity of the last frame
    • GetXPos()                     | Function that returns the X position of 360 points in a last frame
    • GetYPos()                     | Function that returns the Y position of 360 points in a last frame
    • Display(isXY)                 | Function that diplays points frame in the terminal in polar if isXY=false else in cartesian
    • DisplayGraph()                | Function that plots a cartesian graph in the terminal
    • DisplayICP()                  | Function that diplays points without the inf values
    • SaveLidarPoints()             | Function that saves the points of a frame in a *.csv file
    • LidarHelper(context)          | Function to call the lidar outside the class
    • StartThread()                 | Function that starts the thread to receive lidar points
    • ThreadRun()                   | Function that handles the lidar thread
    • StartLidar()                  | Function to start the lidar

*/

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
        uint32_t GetMotorSpeed(void);
        int GetTimeIncrement(void);
        bool GetSat();
        std::vector<int> GetRange(void);
        std::vector<int> GetIntensity(void);
        std::vector<double> GetXPos(void);
        std::vector<double> GetYPos(void);

        void Display(const bool isXY);
        void DisplayGraph();
        void DisplayICP();
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
        std::atomic<bool> m_lidar_endTr;
        std::atomic<bool> m_first_frame;

        int m_port_nr;
        int m_bdrate;

        std::array<std::atomic<int> , 360> m_range;
        std::array<std::atomic<int> , 360> m_intensity;
        std::array<std::atomic<double> , 360> m_xPos;
        std::array<std::atomic<double> , 360> m_yPos;
        std::array<std::atomic<double> , 360> m_xPosSend;
        std::array<std::atomic<double> , 360> m_yPosSend;

        bool m_sat = true;

        uint32_t m_motor_speed = 0;
        int m_time_increment;
        uint16_t rpms;
        int m_cpt = 0;

        Usb *m_usb;
        pthread_t *inc_x_thread;
};
#endif //LIDAR_H
