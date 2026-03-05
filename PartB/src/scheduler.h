//Reid Holloway
//3/5/2026
//CSC-4210
//scheduler.h

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "pcb.h"

int readInput(PCB processes[]);

void runFCFS(PCB processes[], int n);
void runSRTF(PCB processes[], int n);
void runPriority(PCB processes[], int n);
void runRoundRobin(PCB processes[], int n, int quantum);

void printState(int time, PCB *running, PCB *readyQueue[], int rqSize);

#endif
