#include "CPU.h"
#include "printer.h"

CPU::CPU(double printerNum, double diskNum, double memNum) {
    setEmpty(true); 
    totalMem = memNum;
    remaining = memNum;
    printers = printerNum;
    disks = diskNum;
    for(int i = 0; i <= printerNum; i++) { //printer 0 exists, but is unusable
        printer curr(i);
        printerObjs.push_back(curr);
    }
    for(int i = 0; i <= diskNum; i++) { //disk 0 exists, but is unusable
        disk curr(i);
        diskObjs.push_back(curr);
    }
    PCB total;
    total.inSystem = false;
    total.freed = memNum;
    total.used = 0;
    total.start = 1;
    total.finish = memNum;
    RAM.push_back(total);
}

void CPU::setEmpty(bool val) {
    empty = val;
}

bool CPU::isEmpty() {
    return empty;
}

void CPU::newProcess(PCB obj) {
    setEmpty(false);
    obj.inSystem = true;
    if(RAM.size() > 1) {
        combineEmpty(RAM);
    }
    
    for(size_t i=0; i < RAM.size(); i++) {
        if((obj.memorySpace < RAM[i].freed) && (RAM[i].inSystem == false)) {
            int temp1 = RAM[i].finish;
            int temp2 = RAM[i].start;
            RAM[i] = obj;
            RAM[i].start = temp2;
            RAM[i].inSystem = true;
            RAM[i].finish = temp2 + RAM[i].memorySpace - 1;
            RAM[i].used = RAM[i].memorySpace;
            RAM[i].freed = 0;
            emptySpace(RAM[i].finish + 1 , temp1, i); //the remainder of the empty spot
            
            remaining = remaining - RAM[i].memorySpace;
            queue.push(RAM[i]);
            return;
        }
        else if((obj.memorySpace == RAM[i].freed) && (RAM[i].inSystem == false)) {
            RAM[i].pid = obj.pid;
            RAM[i].priority = obj.priority;
            RAM[i].memorySpace = obj.memorySpace;
            RAM[i].inSystem = true;
            remaining = remaining - obj.memorySpace;
            queue.push(RAM[i]);
            return;
        }
    }
    obj.start = totalMem - remaining + 1;
    obj.finish = obj.start + obj.memorySpace - 1;
    
    obj.used = 0;
    obj.freed = obj.memorySpace;
    
    remaining = remaining - obj.memorySpace;
    
    RAM.push_back(obj);
    queue.push(obj);
    
}

void CPU::returnProcess(PCB obj) {
    setEmpty(false);
    queue.push(obj);
}

void CPU::terminateProcess() {
    int tempPID = queue.top().pid;
    for(size_t i=0; i < RAM.size(); i++) {
        if(RAM[i].pid == tempPID) {
            RAM[i].inSystem = false;
            RAM[i].used = 0;
            RAM[i].freed = RAM[i].memorySpace;
            remaining = remaining + RAM[i].memorySpace;
        }
    }
    combineEmpty(RAM);
    queue.pop();
    if(queue.size() == 0) {
        setEmpty(true);
    }
}

void CPU::moveToPrinter(printer& obj) {
    obj.addProcess(queue.top());
    queue.pop();
    if(queue.size() == 0) {
        setEmpty(true);
    }
}

void CPU::moveToDisk(disk& obj) {
    obj.addProcess(queue.top());
    queue.pop();
    if(queue.size() == 0) {
        setEmpty(true);
    }
}

void CPU::showCPUqueue() {
    std::priority_queue<PCB, std::vector<PCB>, Compare> temp = queue;
    std::cout<<"-------------------------------------------------"<< std::endl;
    std::cout<<"Process with PID "<< temp.top().pid <<" is executing in the CPU."<< std::endl;
    temp.pop();
    int i = 1;
    while(!temp.empty()) {
        std::cout<<"Process with PID "<< temp.top().pid <<" is at queue position "<< i <<"."<< std::endl;
        //std::cout<<"-------------------------------------------------"<< std::endl;
        temp.pop();
        i++;
    }
    std::cout<<"-------------------------------------------------"<< std::endl;
}

void CPU::showPrinters() {
    for(int i=1; i <= printers; i++) {
        std::queue<PCB> temp = printerObjs[i].IOqueue;
        if(temp.empty()) {
            std::cout<<""<< std::endl;
            std::cout<<"-------------------------------------------------"<< std::endl;
            std::cout<<"There are no processes in Printer "<< i <<"."<< std::endl;
            std::cout<<"This printer is idle."<< std::endl;
            std::cout<<"-------------------------------------------------"<< std::endl;
            continue;
        }
        std::cout<<""<< std::endl;
        std::cout<<"Printer "<< i <<"-----------------------------------------"<< std::endl;
        std::cout<<"Process with PID "<< temp.front().pid <<" is using the printer."<< std::endl;
        //std::cout<<"-------------------------------------------------"<< std::endl;
        temp.pop();
        int j = 1;
        while(!temp.empty()) {
            std::cout<<"Process with PID "<< temp.front().pid <<" is at queue position "<< j <<"."<< std::endl;
            //std::cout<<"-------------------------------------------------"<< std::endl;
            temp.pop();
            j++;
        }
    }
    std::cout<<"-------------------------------------------------"<< std::endl;
}

void CPU::showDisks() {
    for(int i=1; i <= disks; i++) {
        std::queue<PCB> temp = diskObjs[i].IOqueue;
        if(temp.empty()) {
            std::cout<<""<< std::endl;
            std::cout<<"-------------------------------------------------"<< std::endl;
            std::cout<<"There are no processes in Disk "<< i <<"."<< std::endl;
            std::cout<<"This disk is empty."<< std::endl;
            std::cout<<"-------------------------------------------------"<< std::endl;
            continue;
        }
        std::cout<<""<< std::endl;
        std::cout<<"Disk "<< i <<"-----------------------------------------"<< std::endl;
        std::cout<<"Process with PID "<< temp.front().pid <<" is using the disk."<< std::endl;
        //std::cout<<"-------------------------------------------------"<< std::endl;
        temp.pop();
        int j = 1;
        while(!temp.empty()) {
            std::cout<<"Process with PID "<< temp.front().pid <<" is at queue position "<< j <<"."<< std::endl;
            //std::cout<<"-------------------------------------------------"<< std::endl;
            temp.pop();
            j++;
        }
    }
    std::cout<<"-------------------------------------------------"<< std::endl;
}

void CPU::showMemory() {
    std::cout<<"--------Memory State--------"<< std::endl;
    for(size_t i=0; i < RAM.size(); i++) {
        if(RAM[i].inSystem == false) {
            std::cout<<"From "<< RAM[i].start <<" to "<< RAM[i].finish <<" = Empty(Unused)" << std::endl;
        }
        else if(RAM[i].inSystem == true) {
        std::cout<<"From "<< RAM[i].start <<" to "<< RAM[i].finish <<" = PID "<< RAM[i].pid << std::endl;
        }   
    }
    /*if(remaining != 0) { 
        std::cout<<"From "<< RAM[RAM.size() - 1].finish + 1 <<" to "<< totalMem <<" = Empty(Unused)" << std::endl;
    }*/
    std::cout<<"----------------------------" << std::endl;
}

void CPU::emptySpace(int start, int finish, int position) {
    PCB Espot;
    Espot.inSystem = false;
    Espot.start = start;
    Espot.finish = finish;
    Espot.used = 0;
    Espot.freed = finish - start + 1;
    RAM.insert(RAM.begin() + position + 1, Espot);
    combineEmpty(RAM);
    
}

void CPU::combineEmpty(std::vector<PCB> &RAM) {
    for(size_t i = 0; i < RAM.size() -1 ; i++) {
        if((RAM[i].inSystem == false) && (RAM[i+1].inSystem == false)) {
            RAM[i].finish = RAM[i+1].finish;
            RAM[i].freed = RAM[i].freed + RAM[i+1].freed;
            RAM.erase(RAM.begin() + i + 1);
        }
    }
}