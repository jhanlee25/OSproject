#ifndef _printer
#define _printer 
#include <vector>
#include <queue>
#include <iostream>
#include "PCB.h"

class printer {
  public:
    printer();
    printer(int val);
    void setEmpty(bool val);
    void addProcess(PCB obj);
    PCB completion();
    
    std::queue<PCB> IOqueue; 
  private:
    bool empty;
    int name;
    
    
};

#endif