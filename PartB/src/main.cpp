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
    int n;

    cout << "===== FCFS =====" << endl;
    n = readInput(processes);
    runFCFS(processes, n);

    cout << endl << "===== SRTF =====" << endl;
    n = readInput(processes);
    runSRTF(processes, n);

    cout << endl << "===== PRIORITY =====" << endl;
    n = readInput(processes);
    runPriority(processes, n);

    cout << endl << "===== ROUND ROBIN =====" << endl;
    n = readInput(processes);
    runRR(processes, n, 2);

    return 0;
}
