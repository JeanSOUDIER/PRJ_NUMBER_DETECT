#ifndef LIDAR_H
#define LIDAR_H

#include <iostream>
#include <wiringPi.h>
#include <vector>
#include <pthread.h>
#include <atomic>
#include "Usb.hpp"
#include "mutex.hpp"

class MobileBase;

class Lidar{
        friend class MutexThread;
        friend class MobileBase;

	public:
        Lidar(const int nb_usb, const int bdrate);
        Lidar(const bool start, const int nb_usb, const int bdrate);
        ~Lidar();

        void SetStart(const bool state);
        void Poll(void);

        int GetBdRate(void);
        int GetPortNb(void);
        bool GetStart(void);
        int GetMotorSpeed(void);
        int GetTimeIncrement(void);
        std::vector<int> GetRange(void);
        std::vector<int> GetIntensity(void);
        MutexThread* GetMutex(void);

protected:
        static void* LidarHelper(void *context);
        void* ThreadLidar();

private:
        void StartLidar(void);

        std::atomic<bool> m_start;

        int m_port_nr;
        int m_bdrate;

        std::vector<std::atomic<int>> m_range;
        std::vector<std::atomic<int>> m_intensity;

        int m_motor_speed = 0;
        int m_time_increment;

        Usb *m_usb;
        MutexThread *m_mutex;
};

#endif //LIDAR_H
