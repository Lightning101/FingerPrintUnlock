/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UnlockManager.cpp
 * Author: sean
 * 
 * Created on August 22, 2018, 3:13 AM
 */

#include "UnlockManager.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <string>

#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <cstring>
#include <cerrno>
#include <vector>


#define FALSE 0
#define TRUE 1


UnlockManager::UnlockManager() {
    isActive = false;
    sysState = "initializing";
}

UnlockManager::UnlockManager(const UnlockManager& orig) {
}

UnlockManager::~UnlockManager() {
}


void UnlockManager::startService()
{
    std::cout<<"Started"<<std::endl;
    std::string device = getDevice();
    
    if(device.compare("Device Not Found") == 0)
    {
      std::cout<<"Could Not Find Device"<<std::endl;
      return ;
    }

    std::cout<<"Device Found: "<<device<<std::endl;
  
    this->device = device;
    this->device = this->device.substr(0,device.length()-1);
    isActive = true;
    sysState = "active";
    mainLoop();
        
    
    
}

void UnlockManager::unlockedWait() {
    std::cout << "Device Unlocked" << std::endl;
    sysState = "unlocked";
    
    while (getSystemLockState().find("false") != -1)
    {
      sleep(1);
    }
    sysState = "locked";
    std::cout << "Device locked" << std::endl;
}

std::string UnlockManager::exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

std::string UnlockManager::getSystemLockState()
{
  return exec("echo $(gdbus call -e -d com.canonical.Unity -o /com/canonical/Unity/Session -m com.canonical.Unity.Session.IsLocked | grep -ioP \"(true)|(false)\")");

}

void UnlockManager::unlockSystem()
{
  system("loginctl unlock-session");
}


std::string UnlockManager::getDevice()
{

  std::string output = exec("find /dev/ttyACM* | grep -ioP \"/dev/ttyACM[0-9]\" ");

  if(output.empty() )
  {
    output = "Device Not Found";
  }

  return output;

}

bool UnlockManager::action(char buff[])
{
    std::string output = buff;
    std::cout<<output <<std::endl;
    int size = strlen(buff);
    std::string nums = "";
    for(int i = 0;i<size; i++)
    {
        int c = (int)buff[i];
        if(c >= '0' && c <= '9')
        {
            nums+=c;
        }
    }
    
    if(!nums.empty())
    {
        std::cout<<"number found "<< nums<<std::endl;
            
        int confid = std::stoi( nums );
        if(confid>100)
        {
            std::cout<<"Unlock Called"<<std::endl;
            unlockSystem();
            sysState = "unlocked";
            sleep(1);
            return 1;
        }
    }
    
    
    return 0;
}


void UnlockManager::mainLoop()
{
    using namespace std::placeholders;
    Serial deviceCom(this->device, 9600);
    
    auto f = [&](char buff[]){ return action(buff);};
    
    while(isActive)
    {
        unlockedWait();
        std::cout<<"read Called"<<std::endl;
        deviceCom.readContinous(f);
        
    }
   
}

void UnlockManager::stopService()
{
    isActive = false;
}


