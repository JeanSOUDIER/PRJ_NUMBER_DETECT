#include "mutex.hpp"

/*MutexThread::MutexThread(std::mutex* _mutex) {
    m_mutex = _mutex;

    m_mutex->lock();
    m_state = false;
    m_mutex->unlock();
}*/

MutexThread::MutexThread(const bool _m_state){

    m_mutex = new std::mutex();

    m_mutex->lock();
    m_state = _m_state;
    m_mutex->unlock();

}

/********************************************************/

MutexThread::~MutexThread(){delete m_mutex;}

/********************************************************/


void MutexThread::Write(const bool state) {
    m_mutex->lock();
	m_state = state;
    m_mutex->unlock();
}

bool MutexThread::Read() {
    m_mutex->lock();
    const bool b = m_state;
    m_mutex->unlock();
	return b;
}

/*********************************************************/


bool MutexThread::getState(){return m_state;}
std::mutex* MutexThread::getMutex(){return m_mutex;}

void MutexThread::setState(const bool _state){m_state = _state;}
void MutexThread::setMutex(std::mutex* _mutex){m_mutex = _mutex;}
