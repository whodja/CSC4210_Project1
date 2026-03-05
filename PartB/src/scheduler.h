//Reid Holloway
//3/5/2026
//CSC-4210
//scheduler.h

//This header declares the scheduling functions that the program uses. Each function adds a different scheduling algorithm.
//Algorithms used: FCFS, SRTF, Priority, Round Robin

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "pcb.h"

//Reads processes from file
int readInput(PCB processes[]);

//Scheduling algorithms
void runFCFS(PCB processes[], int n);
void runSRTF(PCB processes[], int n);
void runPriority(PCB processes[], int n);
void runRoundRobin(PCB processes[], int n, int quantum);

//Helper functions
void printState(int time, PCB *running, PCB *readyQueue[], int rqSize);

#endif
