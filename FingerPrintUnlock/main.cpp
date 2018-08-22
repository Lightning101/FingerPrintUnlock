/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: sean
 *
 * Created on August 22, 2018, 3:09 AM
 */

#include <cstdlib>

#include "UnlockManager.h"


using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    UnlockManager unlock;
    unlock.startService();
    
    
    return 0;
}

