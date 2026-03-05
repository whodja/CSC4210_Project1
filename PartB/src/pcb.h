//Reid Holloway
//3/5/2026
//CSC-4210
//pcb.h

#ifndef PCB_H
#define PCB_H

#include <string>
using namespace std;

struct PCB
{
    string pid;
    int arrival;   
    int burst;
    int remaining;
    int priority;

    string state;

    int start_time;
    int completion_time;
};
#endif
