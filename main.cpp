#include <iostream>
#include <cstring>
#include <climits> //for ignoring non numerical input
#include "printer.h"
#include "PCB.h"
#include "CPU.h"
#include "disk.h"

int main() {
/*------------star of sys gen------------*/
    
double printerAmount;
double diskAmount;
double memory;

std::cout<<"Please specify the number of printers in this system: ";
std::cin >> printerAmount;
while ((std::cin.fail()) || (printerAmount <= 0) || (printerAmount != (int)printerAmount)) {
    std::cin.clear(); //reset the cin into a working state
    std::cin.ignore(INT_MAX, '\n'); //ignore previous cin input
    std::cout << "Try Again. Please enter a positive whole NUMBER: ";
    std::cin >> printerAmount;
}

std::cout<<"Please specify the number of disks in this system: ";
std::cin >> diskAmount;
while ((std::cin.fail()) || (diskAmount <= 0) || (diskAmount != (int)diskAmount)) {
    std::cin.clear();
    std::cin.ignore(INT_MAX, '\n');
    std::cout << "Try Again. Please enter a positive whole NUMBER: ";
    std::cin >> diskAmount;
}

std::cout<<"Please specify the amount of memory in this system: ";
std::cin >> memory;
while ((std::cin.fail()) || (diskAmount <= 0) || (memory != (int)memory)) {
    std::cin.clear();
    std::cin.ignore(INT_MAX, '\n');
    std::cout << "Try Again. Please enter a positive whole NUMBER: ";
    std::cin >> memory;
}

CPU runningCPU(printerAmount, diskAmount, memory);

/*------------end of sys gen------------*/

std::cout<<"Okay, you may now input commands for the running section."<< std::endl;

/*------------start of running section------------*/
int counter = 1; //tracks the number of created process
bool loop = true;
while(loop == true) { //keep asking for user command inputs
    char temp[100];
    std::cout<<"Please input your next command: ";
    std::cin >> temp;
    size_t size = strlen(temp);
    
    //arrival of process
    if((temp[0] == 'A') && (size == 1)) { 
        double memorySpace;
        double priority;
        std::cout<<"How much memory does this new process need? "<< std::endl;
        std::cout<<"Please enter: ";
        std::cin>> memorySpace;
        while ((std::cin.fail()) || (memorySpace < 0)  || (memorySpace != (int)memorySpace)) {
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
            std::cout << "Try Again. Please enter a positive whole NUMBER: ";
            std::cin >> memorySpace;
        }
        if(memorySpace > runningCPU.remaining) {
            std::cout <<"There are not enough space in the memory for this process."<< std::endl;
            std::cout <<"Please make a process with less memory requirement."<< std::endl;
            continue;
        }
        std::cout<<"What is the priority of this process?" << std::endl;
        std::cout<<"Please enter: ";
        std::cin>> priority;
        while ((std::cin.fail()) || (priority < 0)  || (priority != (int)priority)) {
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
            std::cout << "Try Again. Please enter a positive whole NUMBER: ";
            std::cin >> priority;
        }
        
        PCB currProcess;
        currProcess.pid = counter;
        currProcess.memorySpace = memorySpace;
        currProcess.priority = priority;
        
        runningCPU.newProcess(currProcess);
        
        std::cout<<"Very well. Process "<< counter <<" is now in the system."<< std::endl;
        counter++;
    }
    //termination of process
    else if((temp[0] == 't') && (size == 1)) {
        if(runningCPU.isEmpty()) {
            std::cout<<"There are no processes in the CPU to terminate." << std::endl;
            continue;
        }
        runningCPU.terminateProcess();
        std::cout<<"Very well. The process currently in the CPU is terminated." << std::endl;
        
        
    }
    //request of printer
    else if((temp[0] == 'p') && (size > 1)) {
        char* temp_chopped = temp + 1; //point to the same cString, but the first letter will be after null
        int num = atoi(temp_chopped); //convert to a int
        if((num > printerAmount || (num == 0))) { //if p0 is entered, or [1] position in non-int
            std::cout<<"Your previous command is incorrect. Try Again. " << std::endl;
            continue;
        }
        else if(runningCPU.isEmpty()) {
            std::cout<<"There are currently no processes in the CPU to use the printer." << std::endl;
            std::cout<<"Please try again." << std::endl;
            continue;
        }
        runningCPU.moveToPrinter(runningCPU.printerObjs[num]);
        
        std::cout<<"Very well. Printer "<< num <<" is requested." << std::endl;
        std::cout<<"The process that was in the CPU is now in the requested printer."<< std::endl;
    }
    //request of disk
    else if((temp[0] == 'd') && (size > 1)) {
        char* temp_chopped = temp + 1;
        int num = atoi(temp_chopped);
        if((num > diskAmount) || (num == 0)) {
            std::cout<<"Your previous command is incorrect. Try Again. " << std::endl;
            continue;
        }
        else if(runningCPU.isEmpty()) {
            std::cout<<"There are currently no processes in the CPU to use the disk." << std::endl;
            std::cout<<"Please try again." << std::endl;
            continue;
        }
        runningCPU.moveToDisk(runningCPU.diskObjs[num]);
        std::cout<<"Very well. Disk "<< num <<" is requested." << std::endl;
        std::cout<<"The process that was in the CPU is now in the requested disk."<< std::endl;
    }
    //completion of printer
    else if((temp[0] == 'P') && (size > 1)) {
        char* temp_chopped = temp + 1;
        int num = atoi(temp_chopped);
        if((num > printerAmount) || (num == 0)) {
            std::cout<<"Your previous command is incorrect. Try Again. " << std::endl;
            continue;
        }
        else if(runningCPU.printerObjs[num].IOqueue.empty()) {
            std::cout<<"There are currently no processes in this printer." << std::endl;
            continue;
        }
        PCB returnProcess = runningCPU.printerObjs[num].completion();
        runningCPU.returnProcess(returnProcess);
        std::cout<<"Very well. The process in Printer "<< num <<" is completed." << std::endl;
    }
    //completion of disk
    else if((temp[0] == 'D') && (size > 1)) {
        char* temp_chopped = temp + 1;
        int num = atoi(temp_chopped);
        if((num > diskAmount) || (num == 0)) {
            std::cout<<"Your previous command is incorrect. Try Again. " << std::endl;
            continue;
        }
        else if(runningCPU.diskObjs[num].IOqueue.empty()) {
            std::cout<<"There are currently no processes in this disk." << std::endl;
            continue;
        }
        PCB returnProcess = runningCPU.diskObjs[num].completion();
        runningCPU.returnProcess(returnProcess);
        std::cout<<"Very well. The process in Disk "<< num <<" is completed." << std::endl;
    }
    //show ready queue and CPU
    else if((temp[0] == 'S') && (temp[1] == 'r') && (size ==2)) {
        if(runningCPU.queue.empty()) {
            std::cout<<"There are no processes in the CPU."<< std::endl;
            std::cout<<"The CPU is idle."<< std::endl;
            continue;
        }
        runningCPU.showCPUqueue();
    }
    //show all I/O
    else if((temp[0] == 'S') && (temp[1] == 'i') && (size ==2)) {
        runningCPU.showPrinters();
        std::cout<<"###################"<< std::endl;
        runningCPU.showDisks();
    }
    //show memory
    else if((temp[0] == 'S') && (temp[1] == 'm') && (size ==2)) {
        /*if(runningCPU.queue.empty()) {
            std::cout<<"There are currently no processes in the memory."<< std::endl;
            continue;
        }*/
        runningCPU.showMemory();
    }
    //invalid commands
    else {
        std::cout<<"Your previous command is incorrect. Try Again. " << std::endl;
    }
}

return 0;
}