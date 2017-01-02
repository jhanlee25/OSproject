#ifndef _CPU
#define _CPU
#include <vector>
#include <queue>
#include "PCB.h"
#include "printer.h"
#include "disk.h"

class CPU {
  public:
    CPU();
    CPU(double printerNum, double diskNum, double memNum);
    void setEmpty(bool val);
    bool isEmpty();
    void newProcess(PCB obj);
    void returnProcess(PCB obj);
    void terminateProcess();
    void moveToPrinter(printer& obj);
    void moveToDisk(disk& obj);
    void showCPUqueue();
    void showPrinters();
    void showDisks();
    void showMemory();
    void emptySpace(int start, int finish, int position);
    void combineEmpty(std::vector<PCB> &RAM);
    
    struct Compare {
      bool operator()(PCB A, PCB B) {
        if (A.priority < B.priority) {return true;}
          return false;
      }
    };
   
    std::priority_queue<PCB, std::vector<PCB>, Compare> queue;
    std::vector<printer> printerObjs;
    std::vector<disk> diskObjs;
    
    std::vector<PCB> RAM;
    int remaining;
    
  private:
    bool empty;
    int totalMem;
    int printers;
    int disks;
    
    
};

#endif