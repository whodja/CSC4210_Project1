//Reid Holloway
//3/5/2026
//CSC-4210
//pcb.h

#ifndef PCB_H
#define PCB_H
#include <string>
using namespace std;

//PCB struct representing a process
struct PCB
{
    string pid;          //Process ID
    int arrival;         //Arrival time
    int burst;           //Total CPU burst time required
    int remaining;       //Remaining CPU time
    int priority;        //Priority level

    string state;        //State: (i.e. NEW, READY, RUNNING, TERMINATED)

    int start_time;      //First time process starts 
    int completion_time; //Time when process finishes 
};
#endif
