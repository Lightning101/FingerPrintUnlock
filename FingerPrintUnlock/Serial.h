/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Serial.h
 * Author: sean
 *
 * Created on August 22, 2018, 3:32 AM
 */

#ifndef SERIAL_H
#define SERIAL_H
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <functional>


class Serial {
public:
    Serial(const std::string device, int baudRate);
    Serial(const Serial& orig);
    virtual ~Serial();
    void readContinous(std::function<bool(char[])> action);
    void readOnce(std::function<bool(char[])> action);
    void restPort();
    
private:
    std::string device;
    int baudRate;
    int getBaudRate(int rate);
    int fd, c, res;
    struct termios oldtio, newtio;
    char buf[255];
    void setup();
};

#endif /* SERIAL_H */

