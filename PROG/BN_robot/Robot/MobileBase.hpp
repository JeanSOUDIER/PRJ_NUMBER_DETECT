#ifndef MOBILEBASE_H
#define MOBILEBASE_H

#include <atomic>

#include <cmath>
#include <complex>
#include <ctime>

#include <iostream>

#include <limits>

#include <pthread.h>

#include <valarray>
#include <vector>
#include <atomic>

#include "Lidar.hpp"
#include "Usb.hpp"
#include "../SequenceHandler_STL/utility.h"
#include "../Algorithms/ICP.hpp"

/**

                     MobileBase class

    DUHAMEL Erwan (erwanduhamel@outlook.com)
    SOUDIER Jean  (jean.soudier@insa-strasbourg.fr)


    Provides a class for Mobile Base control.

    Features :

    • control the mobile base

    Functions :

    • MobileBase                        | Constructor with arguments nb_usb (the number of the USB port), bdrate (the baudrate), the position of the robot <posX,posY,angle> and the lidar if you have one
    • StartPlacing()                    | Function to place the robot when the system is setting up.
    • GetAngleStart()                   | Function that returns the angle of the robot with protections and filter
    • GetPosStart(delay,angle,dist_max) | Function that returns the position of the robot to the board with protections and filter, arguments are : 'delay' if you want to wait 0.5s, 'angle' if you want to adjust the point of interest and the 'dist_max' for the max lentgh in the protection.
    • GoPos(x,y,a)                      | Function to place the robot in space in <x,y> and 'a' is the end angle
    • SetSpeed(speed)                   | Function to set the speed of the motors
    • SetSpeed(L,R)                     | Function to set the speed of the motors
    • SetSpeedCons(speed)               | Function to set speed input for the position loop
    • GetSpeedCons()                    | Function that returns the speed input
    • PrintPos()                        | Function that prints in the terminal the position of the robot
    • GetCurrentPos()                   | Function that returns the position of the robot <x,y,thet>
    • SetCurrentPosi(p)                 | Function to set the current position of the virtual robot for the feedback
    • GetCurrentPosi()                  | Function that returns the position of the robot <x,y,thet>
    • currentPos_helper(context)        | Function to call the current position outside the class
    • currentPos_helper_meter(context)  | Function to call the current position outside the class in meter
    • MobileBaseHelper(context)         | Function to call the thread outside the class
    • StartThread()                     | Function to start the thread to compute the ICP
    • ThreadRun()                       | Function to handle the thread of the ICP
    • GetLidarPoints(nb)                | Function to read the lidar points, if 'nb==true' create a buffer of the last value for the ICP

*/

class MobileBase {
    public:
        explicit MobileBase(const int nb_usb, const int bdrate);
        explicit MobileBase(const double posX, const double posY, const double angle, const int nb_usb, const int bdrate);
        explicit MobileBase(const int nb_usb, const int bdrate, Lidar* RPLidar);
        explicit MobileBase(const double posX, const double posY, const double angle, const int nb_usb, const int bdrate, Lidar* RPLidar);
        ~MobileBase();

        void StartPlacing();
        double GetAngleStart();
        double GetPosStart(bool delai, int angle, double dist_max);
        void GoPos(const double x, const double y, const double a);
        void SetSpeed(std::vector<int> speed);
        void SetSpeed(int L, int R);
        void SetSpeedCons(double speed);
        double GetSpeedCons();
        void PrintPos();
        std::vector<double> GetCurrentPos();
        
        void SetCurrentPosi(std::valarray<double> p);
        std::valarray<double> GetCurrentPosi();
        static std::valarray<double> currentPos_helper(void* context);
        static std::valarray<double> currentPos_helper_meter(void* context);

        static void* MobileBaseHelper(void *context);
        void StartThread();
    protected:
        void* ThreadRun();
        void GetLidarPoints(bool nb);
    private:

        const double START_DIST = 380; //distance to the white board
        const double SPEED_CST = 50;
        const double DIST_BT_WHEEL = 23;
        const double SPEED_NORM = 25.5;
        const double SPEED_ANGLE = SPEED_NORM*DIST_BT_WHEEL*M_PI;

        std::atomic<bool> m_start;
        std::atomic<int> m_timeCont;

        int m_port_nr;
        int m_bdrate;
        bool m_lidar_start;

        Usb *m_usb;
        Lidar *m_RPLidar;
        pthread_t *inc_x_thread;

        std::vector<double> m_posXlid;
        std::vector<double> m_posYlid;
        std::vector<double> m_posXN1lid;
        std::vector<double> m_posYN1lid;

        std::atomic<double> m_posX;
        std::atomic<double> m_posY;
        std::atomic<double> m_angle;
        double m_posXN1 = 0;
        double m_posYN1 = 0;
        double m_angleN1 = 0;
        double m_dist_board = 0;

        double m_speedNorm = 0;
        
        std::valarray<double> m_posEmul;

        std::clock_t m_startTime;
        std::clock_t m_endTime;
};

#endif //MOBILEBASE_H
