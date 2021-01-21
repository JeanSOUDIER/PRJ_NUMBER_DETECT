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

/**

                     Bluetooth class

    DUHAMEL Erwan (erwanduhamel@outlook.com)
    SOUDIER Jean  (jean.soudier@insa-strasbourg.fr)


    Provides a class for Bluetooth communication.

    Features :

    • Send and read data from bluetooth

    Functions :

    • Bluetooth                     | Constructor with arguments nb_usb (the number of the USB port) and bdrate (the baudrate)
    • GetRX()                       | Function that returns the value to read (WARNING : this function waits, i.e. blocks the current thread until something is received)
    • SetTX(txt)                    | Function to send char vector or string
    • BluetoothHelper()             | Function to call function outside
    • StartThread()                 | Function to start the bluetooth thread
    • ThreadRun()                   | Function that handles the thread of the Bluetooth communication
    • ReadThread()                  | Function to read the messages in the current thread 
    • WriteThread()                 | Function to write the messages in the current thread 
    • UpdateThread()                | Function to updates the thread buffers (m_tx to m_buff_tx) and (m_buff_rx to m_rx)
    • GetRXsize()                   | Function that returns the size of the read buffer

*/

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
