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

class MobileBase {
    public:
        explicit MobileBase(const int nb_usb, const int bdrate);
        explicit MobileBase(const double posX, const double posY, const double angle, const int nb_usb, const int bdrate);
        explicit MobileBase(const int nb_usb, const int bdrate, Lidar* RPLidar);
        explicit MobileBase(const double posX, const double posY, const double angle, const int nb_usb, const int bdrate, Lidar* RPLidar);
        ~MobileBase();

        void StartPlacing();
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

        const double START_DIST = 380;
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
