#ifndef _PCB
#define _PCB

struct PCB {
  int pid;
  int priority;
  int memorySpace;
  
  bool inSystem;
  int used;
  int freed;
  
  int start;
  int finish;
};

#endif