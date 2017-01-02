#include "printer.h"

printer::printer(int val) {
    name = val;
}

void printer::setEmpty(bool val) {
    empty = val;
}

void printer::addProcess(PCB obj) {
    setEmpty(false);
    IOqueue.push(obj);
}

PCB printer::completion() {
    PCB curr = IOqueue.front();
    IOqueue.pop();
    return curr;
}