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
#include <cstring>
#else

#include <windows.h>

#endif


namespace RS232{
	
#if defined(__linux__) || defined(__FreeBSD__)
    extern unsigned PORTNR;
    extern std::vector<std::string> comports;

    extern struct termios new_port_settings;
    extern std::vector<struct termios> old_port_settings;
    extern std::vector<int> Cport;
    extern int error;

#else
    extern unsigned PORTNR;
    extern std::vector<std::string> comports;
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
