/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2005 - 2019 Teunis van Beelen
*
* Email: teuniz@protonmail.com
*
*
*****************************************************************
* EDITED BY DUHAMEL ERWAN & SOUDIER JEAN
*
*               October 2020
*
* Emails : erwanduhamel@outlook.com
*          jean.soudier@insa-strasbourg.fr
*
*
* Changelog :
*
*
* • Turned C library into C++
* • Updated library to recent C++ code standards
*
* • Added a namespace
* • Removed dangerous casts and C array, and replaced them by clean C++ casts, std::vector s and std::string s
* • Added convience OpenComport(const std::string &, const unsigned int &baudrate ,const std::string &mode , const bool &flowctrl)
*   that allows to put a std::string as argument without having to mention is number (which is deduced)
*
* • Marqued most arguments as const and passed them by reference
* • Replaced int by unsigned for comport_number arguments.
*
*
*
*
*****************************************************************
*
***************************************************************************
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
***************************************************************************
*/


/* For more info and how to use this library, visit: http://www.teuniz.net/RS-232/ */


#ifndef RS_232_H
#define RS_232_H


#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include <cstddef>

#if defined(__linux__) || defined(__FreeBSD__)

#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <sys/file.h>
#include <errno.h>

#else

#include <windows.h>

#endif


namespace RS232{
	
#if defined(__linux__) || defined(__FreeBSD__)
    static constexpr unsigned PORTNR  = 40;
    static const std::vector<std::string> comports = {
        "/dev/ttyS0","/dev/ttyS1","/dev/ttyS2","/dev/ttyS3","/dev/ttyS4","/dev/ttyS5",
        "/dev/ttyS6","/dev/ttyS7","/dev/ttyS8","/dev/ttyS9","/dev/ttyS10","/dev/ttyS11",
        "/dev/ttyS12","/dev/ttyS13","/dev/ttyS14","/dev/ttyS15","/dev/ttyUSB0",
        "/dev/ttyUSB1","/dev/ttyUSB2","/dev/ttyUSB3","/dev/ttyUSB4","/dev/ttyUSB5",
        "/dev/ttyAMA0","/dev/ttyAMA1","/dev/ttyACM0","/dev/ttyACM1",
        "/dev/rfcomm0","/dev/rfcomm1","/dev/ircomm0","/dev/ircomm1",
        "/dev/cuau0","/dev/cuau1","/dev/cuau2","/dev/cuau3",
        "/dev/cuaU0","/dev/cuaU1","/dev/cuaU2","/dev/cuaU3",
        "/dev/ttyUSB_ARBO", "/dev/ttyUSB_LDS"
        };

    extern struct termios new_port_settings;
    extern std::vector<struct termios> old_port_settings;
    extern std::vector<int> Cport;
    extern int error;

#else
    static constexpr unsigned PORTNR  = 32;
    static const std::vector<std::string> comports ={
    "\\\\.\\COM1",  "\\\\.\\COM2",  "\\\\.\\COM3",  "\\\\.\\COM4",
    "\\\\.\\COM5",  "\\\\.\\COM6",  "\\\\.\\COM7",  "\\\\.\\COM8",
    "\\\\.\\COM9",  "\\\\.\\COM10", "\\\\.\\COM11", "\\\\.\\COM12",
    "\\\\.\\COM13", "\\\\.\\COM14", "\\\\.\\COM15", "\\\\.\\COM16",
    "\\\\.\\COM17", "\\\\.\\COM18", "\\\\.\\COM19", "\\\\.\\COM20",
    "\\\\.\\COM21", "\\\\.\\COM22", "\\\\.\\COM23", "\\\\.\\COM24",
    "\\\\.\\COM25", "\\\\.\\COM26", "\\\\.\\COM27", "\\\\.\\COM28",
    "\\\\.\\COM29", "\\\\.\\COM30", "\\\\.\\COM31", "\\\\.\\COM32"
    };
    extern std::vector<HANDLE> Cport;
    extern std::string mode_str;

#endif

int OpenComport(const unsigned int &comport_number, const unsigned int &baudrate, const std::string &mode, const bool &flowctrl);
int OpenComport(const std::string &, const unsigned int &baudrate ,const std::string &mode , const bool &flowctrl);
void CloseComport(const unsigned &port);

int PollComport(const int &comport_number, std::vector<unsigned char> &buffer, const int &size);

int SendByte(const unsigned &port, unsigned char byte);
int SendBuf(const unsigned &port, std::vector<unsigned char> &buffer);


void cputs(const unsigned &port, const std::string &message);

int IsDCDEnabled(const unsigned &port);
int IsRINGEnabled(const unsigned &port);
int IsCTSEnabled(const unsigned &port);
int IsDSREnabled(const unsigned &port);

void enableDTR(const unsigned &port);
void disableDTR(const unsigned &port);

void enableRTS(const unsigned &port);
void disableRTS(const unsigned &port);

void flushRX(const unsigned &port);
void flushTX(const unsigned &port);

void flushRXTX(const unsigned &port);

int GetPortnr(const std::string &device_name);


}



#endif
