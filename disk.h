#ifndef _disk
#define _disk
#include <vector>
#include <queue>
#include <iostream>
#include "PCB.h"

class disk {
  public:
    disk();
    disk(int val);
    void setEmpty(bool val);
    void addProcess(PCB obj);
    PCB completion();
    
    std::queue<PCB> IOqueue; 
  private:
    bool empty;
    int name;
};

#endif