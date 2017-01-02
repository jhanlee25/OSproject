#include "disk.h"

disk::disk(int val) {
    name = val;
}

void disk::setEmpty(bool val) {
    empty = val;
}

void disk::addProcess(PCB obj) {
    setEmpty(false);
    IOqueue.push(obj);
}

PCB disk::completion() {
    PCB curr = IOqueue.front();
    IOqueue.pop();
    return curr;
}