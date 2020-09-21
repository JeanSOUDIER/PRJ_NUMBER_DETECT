#ifndef LIDAR_H
#define LIDAR_H

#include <iostream>
#include <wiringPi.h>
#include <vector>
#include <pthread.h>
#include "Usb.hpp"
#include "mutex.hpp"

class MobileBase;

class Lidar{
        friend class MutexThread;
        friend class MobileBase;

	public:
		Lidar(const bool start, const int nb_usb, const int bdrate);
                Lidar(const bool start, const int nb_usb, const int bdrate, bool mutex_state);
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

		bool m_start = false;

		int m_port_nr;
                int m_bdrate;

                std::vector<int> m_range = std::vector<int>(360);
                std::vector<int> m_intensity = std::vector<int>(360);

                int m_motor_speed = 0;
                int m_time_increment;

                Usb *m_usb;
                MutexThread *m_mutex;
};

#endif //LIDAR_H