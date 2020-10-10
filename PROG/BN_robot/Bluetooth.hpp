#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <iostream>
#include <string>
#include <algorithm>
#include <pthread.h>
#include <atomic>
#include <deque>
#include "Usb.hpp"

class Bluetooth {
	public:
		Bluetooth(const int nb_usb, const int bdrate);
		~Bluetooth();

		std::vector<unsigned char> GetRX(void);
		void SetTX(std::vector<char> txt);
		void SetTX(std::string txt);

		static void* BluetoothHelper(void *context);
        void StartThread();
protected:
        void* ThreadRun();
		/*unsigned char Read();
		void WriteWord(std::string txt);
		void WriteWord(std::vector<char> txt);*/
	private:
		//bool IsAddrValid(std::string addr);

        std::atomic<bool> m_start;

		std::atomic<bool> m_rec;
		std::atomic<bool> m_sen;

        std::deque<std::array<std::atomic<char>>> m_tx(1,std::array<std::atomic<char>> (256,0));
        std::deque<std::array<std::atomic<unsigned char>>> m_rx(1,std::array<std::atomic<unsigned char>> (256,0));

        int m_length;
        std::vector<unsigned char> m_msg;

		int m_port_nr;
        int m_bdrate;

        Usb *m_usb;
        pthread_t *inc_x_thread;
};

#endif //BLUETOOTH_H