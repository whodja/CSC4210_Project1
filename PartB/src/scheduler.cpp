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
