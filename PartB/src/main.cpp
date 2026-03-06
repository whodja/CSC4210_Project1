//Reid Holloway
//3/5/2026
//CSC-4210
//main.cpp

#include <iostream>
#include "scheduler.h"

using namespace std;

int main()
{
    PCB processes[100];

    //read processes from input file
    int n = readInput(processes);

    cout << "===== FCFS Simulation =====" << endl;

    //For the other algorithms you would call: runSRTF(processes, n); , runPriority(processes, n); , or runRoundRobin(processes, n, 2);
    runFCFS(processes, n);

    return 0;
}
