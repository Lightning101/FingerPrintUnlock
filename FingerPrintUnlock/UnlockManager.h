/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UnlockManager.h
 * Author: sean
 *
 * Created on August 22, 2018, 3:13 AM
 */

#pragma once
#ifndef UNLOCKMANAGER_H
#define UNLOCKMANAGER_H
#include <string>

#include "Serial.h"
#include <functional>

class UnlockManager {
public:
    UnlockManager();
    UnlockManager(const UnlockManager& orig);
    virtual ~UnlockManager();
    void startService();
    void stopService();
    bool action(char buff[]);
    ;
private:
    std::string device;
    int baudRate;
    std::string exec(const char* cmd);
    std::string getSystemLockState();
    void unlockSystem();
    std::string getDevice();
    void unlockedWait();
    void mainLoop();
    bool isActive;
    std::string sysState;
   ;
    
    
};

#endif /* UNLOCKMANAGER_H */

