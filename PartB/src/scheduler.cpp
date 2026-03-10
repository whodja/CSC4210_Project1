//Reid Holloway
//3/5/2026
//CSC-4210
//scheduler.cpp

//This file implements the CPU scheduling algorithms and simulation logic.
//It reads process data, simulates the scheduler step-by-step,
//and prints both the PCB state trace and final statistics.

#include <iostream>
#include <fstream>
#include "scheduler.h"

using namespace std;

//Reads processes from input.txt and loads them into the PCB array
//Returns the number of processes that were read
int readInput(PCB processes[])
{
    ifstream file("input.txt");
    int count = 0;

    //Read each line of the file until EOF
    while(file >> processes[count].pid
               >> processes[count].arrival
               >> processes[count].burst
               >> processes[count].priority)
    {
        //Initialize runtime fields for the simulation
        processes[count].remaining = processes[count].burst; //remaining CPU time
        processes[count].state = "NEW"; //initial state
        processes[count].start_time = -1; //not started yet
        processes[count].completion_time = -1; //not finished yet

        count++; //move to next PCB slot
    }

    file.close();
    return count;
}

//Prints the current state of the CPU and ready queue at a given time
//Used to trace the scheduling simulation step-by-step
void printState(int time, PCB *running, PCB *readyQueue[], int rqSize)
{
    cout << "Time " << time << ":" << endl;
    cout << "RUNNING:" << endl;

    //If no process is currently using the CPU
    if(running == nullptr)
    {
        cout << "IDLE" << endl;
    }
    else
    {
        //Print information about the running process
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

    //Print all processes currently waiting in the ready queue
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

//Print final scheduling statistics after simulation finishes
//Includes waiting time, turnaround time, and response time
void printStats(PCB processes[], int n)
{
    double totalWaiting = 0;
    double totalTurnaround = 0;
    double totalResponse = 0;

    cout << "----- Process Statistics -----" << endl;

    for(int i=0;i<n;i++)
    {
        //Calculate metrics for each process
        int turnaround = processes[i].completion_time - processes[i].arrival;
        int waiting = turnaround - processes[i].burst;
        int response = processes[i].start_time - processes[i].arrival;

        totalWaiting += waiting;
        totalTurnaround += turnaround;
        totalResponse += response;

        //Print per-process statistics
        cout << "PID=" << processes[i].pid
             << " Waiting=" << waiting
             << " Turnaround=" << turnaround
             << " Response=" << response
             << endl;
    }

    cout << endl;

    //Print average metrics
    cout << "Average Waiting Time: " << totalWaiting/n << endl;
    cout << "Average Turnaround Time: " << totalTurnaround/n << endl;
    cout << "Average Response Time: " << totalResponse/n << endl;

    cout << endl;
}

//FCFS Scheduling (First-Come, First-Served)
//Processes are executed in the order they arrive
void runFCFS(PCB processes[], int n)
{
    PCB* readyQueue[100]; //simple array used as queue
    int rqSize = 0;

    PCB* running = nullptr; //currently running process
    int time = 0;           //simulation clock
    int finished = 0;       //number of completed processes

    while(finished < n)
    {
        //Admit processes that arrive at the current time
        for(int i=0;i<n;i++)
        {
            if(processes[i].arrival == time)
            {
                processes[i].state = "READY";
                readyQueue[rqSize++] = &processes[i];
            }
        }

        //If CPU is idle, select the next process in the queue
        if(running == nullptr && rqSize > 0)
        {
            running = readyQueue[0];

            //Shift queue left after removing first element
            for(int i=1;i<rqSize;i++)
                readyQueue[i-1] = readyQueue[i];

            rqSize--;

            running->state = "RUNNING";

            //Record response time if this is the first time running
            if(running->start_time == -1)
                running->start_time = time;
        }

        //Display the current system state
        printState(time, running, readyQueue, rqSize);

        //Execute the running process for 1 time unit
        if(running != nullptr)
        {
            running->remaining--;

            //Check if process finished execution
            if(running->remaining == 0)
            {
                running->state = "TERMINATED";
                running->completion_time = time + 1;
                running = nullptr;
                finished++;
            }
        }

        //Advance simulation clock
        time++;
    }

    printStats(processes, n);
}

//SRTF Scheduling (Shortest Remaining Time First)
//Preemptive version of shortest job first
void runSRTF(PCB processes[], int n)
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

        //Preempt current process by returning it to ready queue
        if(running != nullptr)
        {
            readyQueue[rqSize++] = running;
            running->state = "READY";
            running = nullptr;
        }

        //Find process with the smallest remaining burst time
        int shortestIndex = -1;
        for(int i=0;i<rqSize;i++)
        {
            if(shortestIndex == -1 ||
               readyQueue[i]->remaining <
               readyQueue[shortestIndex]->remaining)
                shortestIndex = i;
        }

        //Select that process to run
        if(shortestIndex != -1)
        {
            running = readyQueue[shortestIndex];

            //Remove it from the ready queue
            for(int i=shortestIndex+1;i<rqSize;i++)
                readyQueue[i-1] = readyQueue[i];

            rqSize--;

            running->state = "RUNNING";

            if(running->start_time == -1)
                running->start_time = time;
        }

        printState(time, running, readyQueue, rqSize);

        //Execute for one time unit
        if(running != nullptr)
        {
            running->remaining--;

            if(running->remaining == 0)
            {
                running->state = "TERMINATED";
                running->completion_time = time + 1;
                running = nullptr;
                finished++;
            }
        }

        time++;
    }

    printStats(processes, n);
}

//Priority Scheduling (non-preemptive)
//Lower priority value means higher priority
void runPriority(PCB processes[], int n)
{
    PCB* readyQueue[100];
    int rqSize = 0;
    PCB* running = nullptr;

    int time = 0;
    int finished = 0;

    while(finished < n)
    {
        //Add processes that arrive at this time
        for(int i=0;i<n;i++)
        {
            if(processes[i].arrival == time)
            {
                processes[i].state = "READY";
                readyQueue[rqSize++] = &processes[i];
            }
        }

        //Choose highest priority process if CPU is idle
        if(running == nullptr && rqSize > 0)
        {
            int bestIndex = 0;

            for(int i=1;i<rqSize;i++)
            {
                if(readyQueue[i]->priority <
                   readyQueue[bestIndex]->priority)
                {
                    bestIndex = i;
                }
            }

            running = readyQueue[bestIndex];

            //Remove it from queue
            for(int i=bestIndex+1;i<rqSize;i++)
                readyQueue[i-1] = readyQueue[i];

            rqSize--;

            running->state = "RUNNING";

            if(running->start_time == -1)
                running->start_time = time;
        }

        printState(time, running, readyQueue, rqSize);

        //Execute process
        if(running != nullptr)
        {
            running->remaining--;

            if(running->remaining == 0)
            {
                running->state = "TERMINATED";
                running->completion_time = time + 1;
                running = nullptr;
                finished++;
            }
        }

        time++;
    }

    printStats(processes, n);
}

//Round Robin Scheduling
//Each process runs for a fixed time quantum before being rotated
void runRR(PCB processes[], int n, int quantum)
{
    PCB* readyQueue[100];
    int rqSize = 0;
    PCB* running = nullptr;

    int time = 0;
    int finished = 0;
    int slice = 0; //tracks how long current process has used CPU

    while(finished < n)
    {
        //Add arriving processes to queue
        for(int i=0;i<n;i++)
        {
            if(processes[i].arrival == time)
            {
                processes[i].state = "READY";
                readyQueue[rqSize++] = &processes[i];
            }
        }

        //If CPU is idle, pull next process from queue
        if(running == nullptr && rqSize > 0)
        {
            running = readyQueue[0];

            for(int i=1;i<rqSize;i++)
                readyQueue[i-1] = readyQueue[i];

            rqSize--;

            running->state = "RUNNING";

            if(running->start_time == -1)
                running->start_time = time;

            slice = 0;
        }

        printState(time, running, readyQueue, rqSize);

        if(running != nullptr)
        {
            running->remaining--;
            slice++;

            //Process finished execution
            if(running->remaining == 0)
            {
                running->state = "TERMINATED";
                running->completion_time = time+1;
                running = nullptr;
                finished++;
                slice = 0;
            }
            //Time slice expired, move process to end of queue
            else if(slice == quantum)
            {
                running->state = "READY";
                readyQueue[rqSize++] = running;
                running = nullptr;
                slice = 0;
            }
        }

        time++;
    }

    printStats(processes, n);
}
