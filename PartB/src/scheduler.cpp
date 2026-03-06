//Reid Holloway
//3/5/2026
//CSC-4210
//scheduler.cpp

//This file implements the CPU scheduling algorithms and sim logic.

#include <iostream>
#include <fstream>
#include "scheduler.h"

using namespace std;

//Reads processes from input.txt
int readInput(PCB processes[])
{
    ifstream file("input.txt");

    int count = 0;

    while(file >> processes[count].pid
               >> processes[count].arrival
               >> processes[count].burst
               >> processes[count].priority)
    {
        //initialize fields
        processes[count].remaining = processes[count].burst;
        processes[count].state = "NEW";
        processes[count].start_time = -1;
        processes[count].completion_time = -1;

        count++;
    }

    file.close();

    return count;
}

//Prints the full PCB trace actively
void printState(int time, PCB *running, PCB *readyQueue[], int rqSize)
{
    cout << "Time " << time << ":" << endl;

    cout << "RUNNING:" << endl;

    if(running == nullptr)
    {
        cout << "IDLE" << endl;
    }
    else
    {
        cout << "PID=" << running->pid
             << " Arr=" << running->arrival
             << " Burst=" << running->burst
             << " Rem=" << running->remaining
             << " Prio=" << running->priority
             << " State=" << running->state
             << endl;
    }

    cout << endl;
    cout << "READY:" << endl;

    for(int i=0;i<rqSize;i++)
    {
        PCB *p = readyQueue[i];

        cout << "PID=" << p->pid
             << " Arr=" << p->arrival
             << " Burst=" << p->burst
             << " Rem=" << p->remaining
             << " Prio=" << p->priority
             << " State=" << p->state
             << endl;
    }

    cout << endl;
}

//FCFS Scheduling
void runFCFS(PCB processes[], int n)
{
    PCB* readyQueue[100];
    int rqSize = 0;

    PCB* running = nullptr;

    int time = 0;
    int finished = 0;

    while(finished < n)
    {
        
        //Admit arriving processes
        for(int i=0;i<n;i++)
        {
            if(processes[i].arrival == time)
            {
                processes[i].state = "READY";
                readyQueue[rqSize++] = &processes[i];
            }
        }
