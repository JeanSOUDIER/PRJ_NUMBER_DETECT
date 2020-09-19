#ifndef MUTEX_H
#define MUTEX_H

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <mutex>

class Lidar;

class MutexThread {

    friend class Lidar;

	public:
        //explicit MutexThread(std::mutex* _mutex = nullptr);
        explicit MutexThread(const bool m_state = false);
        ~MutexThread();

        bool getState();
        std::mutex* getMutex();

        void setState(const bool _state);
        void setMutex(std::mutex *_mutex);

    protected:

        void Write(const bool state);
        bool Read();

	private:



        bool m_state;
        std::mutex *m_mutex;
};

#endif
