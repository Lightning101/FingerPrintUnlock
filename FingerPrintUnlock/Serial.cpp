/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


/* 
 * File:   Serial.cpp
 * Author: sean
 * 
 * Created on August 22, 2018, 3:32 AM
 */

#include "Serial.h"
#include <iostream>
#include <cstdio>

#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)

/* baudrate settings are defined in <asm/termbits.h>, which is
included by <termios.h> */

#define _POSIX_SOURCE 1 /* POSIX compliant source */

#define FALSE 0
#define TRUE 1

volatile int STOP = FALSE;

Serial::Serial(const std::string device, int baudRate) {
    this->baudRate = getBaudRate(baudRate);
    this->device = device;
    setup();
    
    
}

Serial::Serial(const Serial& orig) {
}

Serial::~Serial() {
      /* restore the old port settings */
   
    restPort();
}

void Serial::restPort()
{
    tcsetattr(fd, TCSANOW, &oldtio);
}


void Serial::readContinous(std::function<bool(char[])> action) {
    
    while (true) { /* loop until we have a terminating condition */
        /* read blocks program execution until a line terminating character is
           input, even if more than 255 chars are input. If the number
           of characters read is smaller than the number of chars available,
           subsequent reads will return the remaining chars. res will be set
           to the actual number of characters actually read */
        res = read(fd, buf, 255);
        buf[res] = 0; /* set end of string, so we can printf */
        //printf("%s\n", buf);
        int r = action(buf);
        if(r == 1)
        {
            std::cout<<"Break Called"<<std::endl;
            break;
        }
        
    }
    
  
}

void Serial::readOnce(std::function<bool(char[])> action)
{
    res = read(fd, buf, 255);
    buf[res] = 0; /* set end of string, so we can printf */
    //printf("%s\n", buf);
    action(buf);
};


void Serial::setup()
{
    /*
      Open device for reading and writing and not as controlling tty
      because we don't want to get killed if linenoise sends CTRL-C.
     */
    fd = open(device.c_str(), O_RDWR | O_NOCTTY);
    if (fd < 0) {
        perror(device.c_str());
        exit(-1);
    }

    tcgetattr(fd, &oldtio); /* save current serial port settings */
    bzero(&newtio, sizeof (newtio)); /* clear struct for new port settings */

    /*
      BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
      CRTSCTS : output hardware flow control (only used if the cable has
                all necessary lines. See sect. 7 of Serial-HOWTO)
      CS8     : 8n1 (8bit,no parity,1 stopbit)
      CLOCAL  : local connection, no modem contol
      CREAD   : enable receiving characters
     */
    newtio.c_cflag = this->baudRate | CRTSCTS | CS8 | CLOCAL | CREAD;

    /*
      IGNPAR  : ignore bytes with parity errors
      ICRNL   : map CR to NL (otherwise a CR input on the other computer
                will not terminate input)
      otherwise make device raw (no other input processing)
     */
    //newtio.c_iflag = IGNPAR | ICRNL;
    newtio.c_iflag = IGNPAR;

    /*
     Raw output.
     */
    newtio.c_oflag = OCRNL;

    /*
      ICANON  : enable canonical input
      disable all echo functionality, and don't send signals to calling program
     */
    newtio.c_lflag = ICANON;

    /*
      initialize all control characters
      default values can be found in /usr/include/termios.h, and are given
      in the comments, but we don't need them here
     */
    newtio.c_cc[VINTR] = 0; /* Ctrl-c */
    newtio.c_cc[VQUIT] = 0; /* Ctrl-\ */
    newtio.c_cc[VERASE] = 0; /* del */
    newtio.c_cc[VKILL] = 0; /* @ */
    newtio.c_cc[VEOF] = 4; /* Ctrl-d */
    newtio.c_cc[VTIME] = 0; /* inter-character timer unused */
    newtio.c_cc[VMIN] = 1; /* blocking read until 1 character arrives */
    newtio.c_cc[VSWTC] = 0; /* '\0' */
    newtio.c_cc[VSTART] = 0; /* Ctrl-q */
    newtio.c_cc[VSTOP] = 0; /* Ctrl-s */
    newtio.c_cc[VSUSP] = 0; /* Ctrl-z */
    newtio.c_cc[VEOL] = 0; /* '\0' */
    newtio.c_cc[VREPRINT] = 0; /* Ctrl-r */
    newtio.c_cc[VDISCARD] = 0; /* Ctrl-u */
    newtio.c_cc[VWERASE] = 0; /* Ctrl-w */
    newtio.c_cc[VLNEXT] = 0; /* Ctrl-v */
    newtio.c_cc[VEOL2] = 0; /* '\0' */

    /*
      now clean the modem line and activate the settings for the port
     */
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);
}



int Serial::getBaudRate(int rate) {
    switch (rate) {
        case 0: return B0;
        case 50: return B50;
        case 75: return B75;
        case 110: return B110;
        case 134: return B134;
        case 150: return B150;
        case 200: return B200;
        case 300: return B300;
        case 600: return B600;
        case 1200: return B1200;
        case 1800: return B1800;
        case 2400: return B2400;
        case 4800: return B4800;
        case 9600: return B9600;
        case 38400: return B38400;
        default: return B0;
    };
}

