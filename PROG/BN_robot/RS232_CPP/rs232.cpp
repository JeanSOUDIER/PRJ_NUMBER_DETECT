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
* • Added convience OpenComport(const std::string &portname, const unsigned int &baudrate ,const std::string &mode , const bool &flowctrl)
*   that allows to put a std::string as argument without having to mention the port number (which is deduced)
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


#include "rs232.hpp"


#if defined(__linux__) || defined(__FreeBSD__)   /* Linux & FreeBSD */

unsigned RS232::PORTNR = 41;
std::vector<std::string> RS232::comports = {
        "/dev/ttyS0","/dev/ttyS1","/dev/ttyS2","/dev/ttyS3","/dev/ttyS4","/dev/ttyS5",
        "/dev/ttyS6","/dev/ttyS7","/dev/ttyS8","/dev/ttyS9","/dev/ttyS10","/dev/ttyS11",
        "/dev/ttyS12","/dev/ttyS13","/dev/ttyS14","/dev/ttyS15","/dev/ttyUSB0",
        "/dev/ttyUSB1","/dev/ttyUSB2","/dev/ttyUSB3","/dev/ttyUSB4","/dev/ttyUSB5",
        "/dev/ttyAMA0","/dev/ttyAMA1","/dev/ttyACM0","/dev/ttyACM1",
        "/dev/rfcomm0","/dev/rfcomm1","/dev/ircomm0","/dev/ircomm1",
        "/dev/cuau0","/dev/cuau1","/dev/cuau2","/dev/cuau3",
        "/dev/cuaU0","/dev/cuaU1","/dev/cuaU2","/dev/cuaU3",
        "/dev/ttyUSB_ARBO", "/dev/ttyUSB_LDS", "/dev/ttyTHS1"
};


int RS232::error = 0;
std::vector<int> RS232::Cport(RS232::PORTNR);

struct termios RS232::new_port_settings;
std::vector<struct termios> RS232::old_port_settings(RS232::PORTNR);

int RS232::OpenComport(const unsigned int &comport_number, const unsigned int &baudrate, const std::string &mode, const bool &flowctrl){
  int baudr;
  int status;

  if((comport_number>=RS232::PORTNR)){
    std::cout << ("illegal comport number\n");
    return(1);
  }

  switch(baudrate){
    case      50 : {baudr = B50; break;}
    case      75 : {baudr = B75; break;}
    case     110 : {baudr = B110; break;}
    case     134 : {baudr = B134; break;}
    case     150 : {baudr = B150; break;}
    case     200 : {baudr = B200; break;}
    case     300 : {baudr = B300; break;}
    case     600 : {baudr = B600; break;}
    case    1200 : {baudr = B1200; break;}
    case    1800 : {baudr = B1800; break;}
    case    2400 : {baudr = B2400; break;}
    case    4800 : {baudr = B4800; break;}
    case    9600 : {baudr = B9600; break;}
    case   19200 : {baudr = B19200; break;}
    case   38400 : {baudr = B38400; break;}
    case   57600 : {baudr = B57600; break;}
    case  115200 : {baudr = B115200; break;}
    case  230400 : {baudr = B230400; break;}
    case  460800 : {baudr = B460800; break;}
    case  500000 : {baudr = B500000; break;}
    case  576000 : {baudr = B576000; break;}
    case  921600 : {baudr = B921600; break;}
    case 1000000 : {baudr = B1000000; break;}
    case 1152000 : {baudr = B1152000; break;}
    case 1500000 : {baudr = B1500000; break;}
    case 2000000 : {baudr = B2000000; break;}
    case 2500000 : {baudr = B2500000; break;}
    case 3000000 : {baudr = B3000000; break;}
    case 3500000 : {baudr = B3500000; break;}
    case 4000000 : {baudr = B4000000; break;}
    case 12000000: {custom_bauds = true; baudr = B38400; break; }
    default      : {
                   std::cout << ("invalid baudrate\n");
                   return(1);
                   }
  }

  int cbits = CS8;
  int cpar = 0;
  int ipar = IGNPAR;
  int bstop = 0;

  if((mode.size()) != 3){
    std::cout << "invalid mode: " << mode << "\n";
    return(1); 
  }

  switch(mode[0]){
    case '8': {cbits = CS8; break;}
    case '7': {cbits = CS7; break;}
    case '6': {cbits = CS6; break;}
    case '5': {cbits = CS5; break;}
    default : {
      std::cout << "invalid number of data-bits: " << mode[0];
      return(1);
      }
  }

  switch(mode[1]){
    case 'N':{
              cpar = 0;
              ipar = IGNPAR;
              break;
              }
    case 'n': {
               cpar = 0;
               ipar = IGNPAR;
               break;
               }
               
               
    case 'E':{
               cpar = PARENB;
               ipar = INPCK;
               break;
             }
    case 'e': {
               cpar = PARENB;
               ipar = INPCK;
               break;
             }
               
               
    case 'O':{
               cpar = (PARENB | PARODD);
               ipar = INPCK;
               break;
             }
    case 'o': {
               cpar = (PARENB | PARODD);
               ipar = INPCK;
               break;
              }
               
               
    default : {
      std::cout << "invalid parity " << mode[1] << "\n";
      return(1);
    }
  }

  switch(mode[2]){
    case '1': {bstop = 0; break;}
    case '2': {bstop = CSTOPB; break;}
    default : {
      std::cout << "invalid number of stop bits: " << mode[2] << "\n";
      return(1);
    }
  }

  RS232::Cport[comport_number] = open(&RS232::comports[comport_number][0], O_RDWR | O_NOCTTY | O_NDELAY);
  if(RS232::Cport[comport_number]==-1){
    std::cerr << ("unable to open comport");
    return(1);
  }

  //lock access so that another process can't also use the port
  if(flock(RS232::Cport[comport_number], LOCK_EX | LOCK_NB) != 0){
    close(RS232::Cport[comport_number]);
    std::cerr << ("Another process has locked the comport.");
    return(1);
  }

  error = tcgetattr(RS232::Cport[comport_number], &old_port_settings[0] + comport_number);
  if(error==-1){
    close(RS232::Cport[comport_number]);
    flock(RS232::Cport[comport_number], LOCK_UN);  // free the port so that others can use it.
    std::cerr << ("unable to read portsettings ");
    return(1);
  }

  memset(&new_port_settings, 0, sizeof(new_port_settings));  //clear the new struct

  new_port_settings.c_cflag = cbits | cpar | bstop | CLOCAL | CREAD;
  if(flowctrl){
    new_port_settings.c_cflag |= CRTSCTS;
  }
  new_port_settings.c_iflag = ipar;
  new_port_settings.c_oflag = 0;
  new_port_settings.c_lflag = 0;
  new_port_settings.c_cc[VMIN] = 0;      // block untill n bytes are received
  new_port_settings.c_cc[VTIME] = 0;     //block untill a timer expires (n * 100 mSec.)

  cfsetispeed(&new_port_settings, baudr);
  cfsetospeed(&new_port_settings, baudr);

  error = tcsetattr(RS232::Cport[comport_number], TCSANOW, &new_port_settings);
  if(error==-1){
    tcsetattr(RS232::Cport[comport_number], TCSANOW, &old_port_settings[0] + comport_number);
    close(RS232::Cport[comport_number]);
    flock(RS232::Cport[comport_number], LOCK_UN);  // free the port so that others can use it.
    std::cerr << ("unable to adjust portsettings ");
    return(1);
  }

  if(ioctl(RS232::Cport[comport_number], TIOCMGET, &status) == -1){
    tcsetattr(RS232::Cport[comport_number], TCSANOW, &old_port_settings[0] + comport_number);
    flock(RS232::Cport[comport_number], LOCK_UN);  /* free the port so that others can use it. */
    std::cerr << ("unable to get portstatus");
    return(1);
  }

  status |= TIOCM_DTR;    /* turn on DTR */
  status |= TIOCM_RTS;    /* turn on RTS */

  if(ioctl(RS232::Cport[comport_number], TIOCMSET, &status) == -1){
    tcsetattr(RS232::Cport[comport_number], TCSANOW, &old_port_settings[0] + comport_number);
    flock(RS232::Cport[comport_number], LOCK_UN);  /* free the port so that others can use it. */
    std::cerr << ("unable to set portstatus");
    return(1);
  }

  return(0);
}


int RS232::OpenComport(const std::string &str, const unsigned int &baudrate , const std::string &mode , const bool &flowctrl){
  
    const std::ptrdiff_t index =
    (std::find(RS232::comports.begin(), RS232::comports.end(), "/dev/"+str) != RS232::comports.end())
    ?
    std::distance(RS232::comports.begin(), std::find(RS232::comports.begin(), RS232::comports.end(), "/dev/"+str))
    : -1;


    if(index == -1){
      RS232::comports.push_back( "/dev/"+str);
      RS232::PORTNR++;
      return  RS232::OpenComport(RS232::comports.size()-1 ,baudrate ,mode , flowctrl);

    }
    return  RS232::OpenComport(index ,baudrate ,mode , flowctrl);


  
}

int RS232::PollComport(const int &comport_number, unsigned char *buffer, const int &size){
  int n;

  n = read(RS232::Cport[comport_number], buffer, size);

  if(n < 0){
    if(errno == EAGAIN){return 0;}
  }

  return(n);
}


int RS232::SendByte(const unsigned &port, unsigned char byte){
  //std::cout << static_cast<int>(byte) << " ";
  int n = write(RS232::Cport[port], &byte, 1);
  if(n < 0){
    if(errno == EAGAIN){
      return 0;
    }
    else{
      return 1;
    }
  }

  return(0);
}


int RS232::SendBuf(const unsigned &port, std::vector<unsigned char> &buffer){

  int n = write(RS232::Cport[port], &buffer[0], buffer.size());

  if(n < 0){
    if(errno == EAGAIN){return 0;}
    else{return -1;}
  }

  return(n);
}


void RS232::CloseComport(const unsigned &port){
  int status;

  if(ioctl(RS232::Cport[port], TIOCMGET, &status) == -1){
    std::cerr << ("unable to get portstatus");
  }

  status &= ~TIOCM_DTR;    // turn off DTR
  status &= ~TIOCM_RTS;    // turn off RTS

  if(ioctl(RS232::Cport[port], TIOCMSET, &status) == -1){
    std::cerr << ("unable to set portstatus");
  }

  tcsetattr(RS232::Cport[port], TCSANOW, &old_port_settings[0] + port);
  close(RS232::Cport[port]);

  flock(RS232::Cport[port], LOCK_UN);  //free the port so that others can use it.
}


//Constant  Description
//TIOCM_LE        DSR (data set ready/line enable)
//TIOCM_DTR       DTR (data terminal ready)
//TIOCM_RTS       RTS (request to send)
//TIOCM_ST        Secondary TXD (transmit)
//TIOCM_SR        Secondary RXD (receive)
//TIOCM_CTS       CTS (clear to send)
//TIOCM_CAR       DCD (data carrier detect)
//TIOCM_CD        see TIOCM_CAR
//TIOCM_RNG       RNG (ring)
//TIOCM_RI        see TIOCM_RNG
//TIOCM_DSR       DSR (data set ready)

//http://man7.org/linux/man-pages/man4/tty_ioctl.4.html


int RS232::IsDCDEnabled(const unsigned &port){
    
  int status;

  ioctl(RS232::Cport[port], TIOCMGET, &status);

  if(status&TIOCM_CAR){return(1);}
  else{return(0);}
}


int RS232::IsRINGEnabled(const unsigned &port){
    
  int status;

  ioctl(RS232::Cport[port], TIOCMGET, &status);

  if(status&TIOCM_RNG){return(1);}
  else {return(0);}
}


int RS232::IsCTSEnabled(const unsigned &port){
    
  int status;

  ioctl(RS232::Cport[port], TIOCMGET, &status);

  if(status&TIOCM_CTS){return(1);}
  else{return(0);}
}


int RS232::IsDSREnabled(const unsigned &port){
  int status;

  ioctl(RS232::Cport[port], TIOCMGET, &status);

  if(status&TIOCM_DSR) {return(1);}
  else{return(0);}
}


void RS232::enableDTR(const unsigned &port){
  int status;

  if(ioctl(RS232::Cport[port], TIOCMGET, &status) == -1){std::cerr << ("unable to get portstatus\n");}

  status |= TIOCM_DTR;    //turn on DTR

  if(ioctl(RS232::Cport[port], TIOCMSET, &status) == -1){std::cerr << ("unable to set portstatus\n");}
}


void RS232::disableDTR(const unsigned &port){
  int status;

  if(ioctl(RS232::Cport[port], TIOCMGET, &status) == -1){std::cerr << ("unable to get portstatus\n");}

  status &= ~TIOCM_DTR;    //turn off DTR

  if(ioctl(RS232::Cport[port], TIOCMSET, &status) == -1){std::cerr << ("unable to set portstatus\n");}
}


void RS232::enableRTS(const unsigned &port){
  int status;

  if(ioctl(RS232::Cport[port], TIOCMGET, &status) == -1){std::cerr << ("unable to get portstatus\n");}

  status |= TIOCM_RTS;    //turn on RTS

  if(ioctl(RS232::Cport[port], TIOCMSET, &status) == -1){std::cerr << ("unable to set portstatus\n");}
}


void RS232::disableRTS(const unsigned &port){
  int status;

  if(ioctl(RS232::Cport[port], TIOCMGET, &status) == -1){std::cerr << ("unable to get portstatus");}

  status &= ~TIOCM_RTS;    //turn off RTS

  if(ioctl(RS232::Cport[port], TIOCMSET, &status) == -1){std::cerr << ("unable to set portstatus");}
}


void RS232::flushRX(const unsigned &port){tcflush(RS232::Cport[port], TCIFLUSH);}


void RS232::flushTX(const unsigned &port){tcflush(RS232::Cport[port], TCOFLUSH);}


void RS232::flushRXTX(const unsigned &port){tcflush(RS232::Cport[port], TCIOFLUSH);}


#else  // windows

unsigned RS232::PORTNR = 32;
std::vector<HANDLE> RS232::Cport(RS232::PORTNR);
std::string RS232::mode_str = "";
std::vector<std::string> RS232::comports ={
    "\\\\.\\COM1",  "\\\\.\\COM2",  "\\\\.\\COM3",  "\\\\.\\COM4",
    "\\\\.\\COM5",  "\\\\.\\COM6",  "\\\\.\\COM7",  "\\\\.\\COM8",
    "\\\\.\\COM9",  "\\\\.\\COM10", "\\\\.\\COM11", "\\\\.\\COM12",
    "\\\\.\\COM13", "\\\\.\\COM14", "\\\\.\\COM15", "\\\\.\\COM16",
    "\\\\.\\COM17", "\\\\.\\COM18", "\\\\.\\COM19", "\\\\.\\COM20",
    "\\\\.\\COM21", "\\\\.\\COM22", "\\\\.\\COM23", "\\\\.\\COM24",
    "\\\\.\\COM25", "\\\\.\\COM26", "\\\\.\\COM27", "\\\\.\\COM28",
    "\\\\.\\COM29", "\\\\.\\COM30", "\\\\.\\COM31", "\\\\.\\COM32"
    };

int RS232::OpenComport(const unsigned int &comport_number, const unsigned int &baudrate, const std::string &mode, const bool &flowctrl){

  if((comport_number>=RS232::PORTNR)){
    std::cout << ("illegal comport number\n");
    return(1);
}

  switch(baudrate){
    case     (110) : {mode_str ="baud=110"; break;}
    case     (300) : {mode_str = "baud=300"; break;}
    case     (600) : {mode_str = "baud=600"; break;}
    case    (1200) : {mode_str = "baud=1200"; break;}
    case    (2400) : {mode_str = "baud=2400"; break;}
    case    (4800) : {mode_str = "baud=4800"; break;}
    case   (9600)  : {mode_str = "baud=9600"; break;}
    case   (19200) : {mode_str = "baud=19200"; break;}
    case   (38400) : {mode_str = "baud=38400"; break;}
    case   (57600) : {mode_str = "baud=57600"; break;}
    case  (115200) : {mode_str = "baud=115200"; break;}
    case  (128000) : {mode_str = "baud=128000"; break;}
    case  (256000) : {mode_str = "baud=256000"; break;}
    case  (500000) : {mode_str = "baud=500000"; break;}
    case  (921600) : {mode_str = "baud=921600"; break;}
    case (1000000) : {mode_str = "baud=1000000"; break;}
    case (1500000) : {mode_str = "baud=1500000"; break;}
    case (2000000) : {mode_str = "baud=2000000"; break;}
    case (3000000) : {mode_str = "baud=2000000"; break;}
    case (12000000): {mode_str = "baud=12000000"; break; }

    default      : {std::cout << ("invalid baudrate\n"); return(1);}
  }

  if(mode.length() != 3){
    std::cout << "invalid mode "<< mode << "\n";
    return(1);
  }

  switch(mode[0]){
    case '8': {mode_str+= " data=8"; break;}
    case '7': {mode_str+=" data=7"; break;}
    case '6': {mode_str+=" data=6"; break;}
    case '5': {mode_str+= " data=5"; break;}
    default : {std::cout << "invalid number of data-bits " << mode[0] << "\n";
              return(1);}
  }

  switch(mode[1]){
    case 'N': {mode_str+= " parity=n"; break;}
    case 'n': {mode_str+= " parity=n"; break;}
    case 'E': {mode_str+= " parity=e"; break;}
    case 'e': {mode_str+= " parity=e"; break;}
    case 'O': {mode_str+= " parity=o"; break;}
    case 'o': {mode_str+= " parity=o"; break;}
    default : {std::cout << "invalid parity " << mode[1] << "\n"; return(1);}
  }

  switch(mode[2]){
    case '1': {mode_str+= " stop=1"; break;}
    case '2': {mode_str+= " stop=2"; break;}
    default : {std::cout << "invalid number of stop bits" << mode[2] << "\n"; return(1);}
  }

  if(flowctrl){mode_str+= " xon=off to=off odsr=off dtr=on rts=off";}
  else{mode_str+= " xon=off to=off odsr=off dtr=on rts=on";}


  RS232::Cport[comport_number] = CreateFileA(
                      &RS232::comports[comport_number][0],
                      GENERIC_READ|GENERIC_WRITE,
                      0,                          // no share
                      NULL,                      // no security
                      OPEN_EXISTING,
                      0,                          // no threads
                      NULL);                     //no templates

  if(RS232::Cport[comport_number]==INVALID_HANDLE_VALUE){
    std::cout << ("unable to open comport\n");
    return(1);
  }

  DCB port_settings;
  memset(&port_settings, 0, sizeof(port_settings));  //clear the new struct
  port_settings.DCBlength = sizeof(port_settings);

  if(!BuildCommDCBA(&mode_str[0], &port_settings)){
    std::cout << ("unable to set comport dcb settings\n");
    CloseHandle(RS232::Cport[comport_number]);
    return(1);
  }

  if(flowctrl){
    port_settings.fOutxCtsFlow = TRUE;
    port_settings.fRtsControl = RTS_CONTROL_HANDSHAKE;
  }

  if(!SetCommState(RS232::Cport[comport_number], &port_settings)){
    std::cout << ("unable to set comport cfg settings\n");
    CloseHandle(RS232::Cport[comport_number]);
    return(1);
  }

  COMMTIMEOUTS Cptimeouts;

  Cptimeouts.ReadIntervalTimeout         = MAXDWORD;
  Cptimeouts.ReadTotalTimeoutMultiplier  = 0;
  Cptimeouts.ReadTotalTimeoutConstant    = 0;
  Cptimeouts.WriteTotalTimeoutMultiplier = 0;
  Cptimeouts.WriteTotalTimeoutConstant   = 0;

  if(!SetCommTimeouts(RS232::Cport[comport_number], &Cptimeouts)){
    std::cout << ("unable to set comport time-out settings\n");
    CloseHandle(RS232::Cport[comport_number]);
    return(1);
  }

  return(0);
}


int RS232::OpenComport(const std::string &str, const unsigned int &baudrate , const std::string &mode , const bool &flowctrl){

    const std::ptrdiff_t index =
    (std::find(RS232::comports.begin(), RS232::comports.end(), "\\\\.\\"+str) != RS232::comports.end())
    ?
    std::distance(RS232::comports.begin(), std::find(RS232::comports.begin(), RS232::comports.end(), "\\\\.\\COM"+str))
    : -1;


    if(index == -1){
        RS232::comports.push_back("\\\\.\\COM"+str);
        RS232::PORTNR++;
        return RS232::OpenComport(RS232::comports.size()-1 ,baudrate ,mode , flowctrl);

    }

    return RS232::OpenComport(index ,baudrate ,mode , flowctrl);
}

int RS232::PollComport(const int &comport_number, unsigned char *buffer, const int &size){

  int n;
  //ReadFile(RS232::Cport[comport_number], buffer, size, (LPDWORD)((void *)&n), NULL);
    ReadFile(RS232::Cport[comport_number], buffer, size, LPDWORD(&n), NULL);
  return(n);
}


int RS232::SendByte(const unsigned &port, unsigned char byte){

  int n;

  WriteFile(RS232::Cport[port], &byte, 1, LPDWORD(&n), NULL);

  if(n<0){return(1);}

  return(0);
}


int RS232::SendBuf(const unsigned &port, std::vector<unsigned char> &buffer){

  int n;

  if(WriteFile(RS232::Cport[port], &buffer[0], buffer.size(), LPDWORD(&n), NULL)){return(n);}

  return(-1);
}


void RS232::CloseComport(const unsigned &port){CloseHandle(RS232::Cport[port]);}


int RS232::IsDCDEnabled(const unsigned &port){

  int status;

  GetCommModemStatus(RS232::Cport[port], (LPDWORD(&status)));

  if(status&MS_RLSD_ON){return(1);}
  else{return(0);}
}


int RS232::IsRINGEnabled(const unsigned &port){

  int status;

  GetCommModemStatus(RS232::Cport[port], (LPDWORD(&status)));

  if(status&MS_RING_ON){return(1);}
  else{return(0);}
}


int RS232::IsCTSEnabled(const unsigned &port){

  int status;

  GetCommModemStatus(RS232::Cport[port], (LPDWORD(&status)));

  if(status&MS_CTS_ON){return(1);}
  else {return(0);}
}


int RS232::IsDSREnabled(const unsigned &port){

  int status;

  GetCommModemStatus(RS232::Cport[port], (LPDWORD(&status)));

  if(status&MS_DSR_ON){return(1);}
  else{return(0);}
}


void RS232::enableDTR(const unsigned &port){EscapeCommFunction(RS232::Cport[port], SETDTR);}


void RS232::disableDTR(const unsigned &port){EscapeCommFunction(RS232::Cport[port], CLRDTR);}


void RS232::enableRTS(const unsigned &port){EscapeCommFunction(RS232::Cport[port], SETRTS);}


void RS232::disableRTS(const unsigned &port){EscapeCommFunction(RS232::Cport[port], CLRRTS);}


void RS232::flushRX(const unsigned &port){PurgeComm(RS232::Cport[port], PURGE_RXCLEAR | PURGE_RXABORT);}


void RS232::flushTX(const unsigned &port){PurgeComm(RS232::Cport[port], PURGE_TXCLEAR | PURGE_TXABORT);}


void RS232::flushRXTX(const unsigned &port){
  PurgeComm(RS232::Cport[port], PURGE_RXCLEAR | PURGE_RXABORT);
  PurgeComm(RS232::Cport[port], PURGE_TXCLEAR | PURGE_TXABORT);
}


#endif

void RS232::cputs(const unsigned &port, const std::string &message, const bool &null_terminated){ //Sends a string to serial port

  std::vector<unsigned char> data_vector;
  std::copy(message.begin() , message.end() , std::back_inserter(data_vector));
  if(null_terminated){data_vector.push_back('\0');}
  RS232::cputs(port , data_vector);
  std::cout << std::endl;
}


void RS232::cputs(const unsigned &port, const std::vector<unsigned char> &message){ //Sends a string to serial port
  
  for(unsigned char character:message){RS232::SendByte(port, character);}
  std::cout << std::endl;
  
}


/* return index in RS232::comports matching to device name or -1 if not found */
int RS232::GetPortnr(const std::string &device_name){

#if defined(__linux__) || defined(__FreeBSD__)   /* Linux & FreeBSD */
  std::string port_name = "/dev/" + device_name;
#else  /* windows */
  std::string port_name = "\\\\.\\" + device_name;
#endif
  //port_name[31] = 0;

  for(unsigned i = 0 ; i < RS232::PORTNR ; i++){
    if((RS232::comports[i] == port_name)){return (i+1);}
  }

  return -1;  /* device not found */
}
