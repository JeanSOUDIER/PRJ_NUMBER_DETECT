#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <iostream>
#include <string>
#include <algorithm>
#include <pthread.h>
#include <atomic>
#include <array>
#include <deque>
#include "Usb.hpp"

//#define DEBUG
#ifdef DEBUG
	#define DEBUG_BLUETOOTH_PRINT(x) std::cout<<x<<std::endl;
#else
	#define DEBUG_BLUETOOTH_PRINT(x)
#endif

class Bluetooth {
	public:
		Bluetooth(const int nb_usb, const int bdrate);
		~Bluetooth();

		std::vector<char> GetRX(void);
		void SetTX(std::vector<char> txt);
		void SetTX(std::string txt);

		static void* BluetoothHelper(void *context);
                void StartThread();
        protected:
                void* ThreadRun();
                void ReadThread();
                void WriteThread();
                void UpdateThread();
                int GetRXsize();
	private:

                std::atomic<bool> m_start;
                std::atomic<bool> m_rec;
                std::atomic<bool> m_sen;

                std::array<std::atomic<char>, 256> m_tx;
                std::array<std::atomic<char>, 256> m_rx;

                std::vector<std::vector<char>> m_buff_tx;
                std::vector<std::vector<char>> m_buff_rx;

                int m_length;
                int m_stateR;
                int m_stateS;
                int m_ccR;
                int m_ccS;
                int m_cpt;
                int m_len;
                int m_tx_size;
                std::vector<char> m_msg;

        	int m_port_nr;
                int m_bdrate;

                Usb *m_usb;
                pthread_t *inc_x_thread;
};

#endif //BLUETOOTH_H
