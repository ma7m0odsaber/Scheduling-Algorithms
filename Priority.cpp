
#include "Scheduler.h"

// This function is used for sorting the processes by priority order
// Returns true if P1 < P2 (i.e. P1 Priority is less than P2), and false otherwise
static bool comparePriority(PCB P1, PCB P2)
{

    if(P1.priority == P2.priority) return P1.arrivalTime < P2.arrivalTime ;
    return P1.priority < P2.priority ;
}


// Priority algorithm
void Scheduler::runPriority(vector<PCB> processes)
{
    queue<PCB> readyQueue;
    PCB runningProcess;
    ull currentTime = 0;

    // sort based on Priority
    sort(processes.begin(), processes.end(), comparePriority);

    // Add all processes sorted by priority to the ready queue
    for (auto p : processes)
        readyQueue.push(p);

    // Service the processes one by one
    while (!readyQueue.empty())
    {
        // Remove the least priority process from the ready queue and schedule it for running
        runningProcess = readyQueue.front();
        readyQueue.pop();

        // The process will run to completion as one slice
        ExecutionSlice executionSlice;
        executionSlice.PID = runningProcess.PID;
        executionSlice.length = runningProcess.burstTime;
        executionSlice.startTime = max(currentTime,runningProcess.arrivalTime);
        executionSlice.endTime = executionSlice.startTime + runningProcess.burstTime;
        processingHistory.push_back(executionSlice);
        runningProcess.remainingTime = 0;

        cpuIdleTime += executionSlice.startTime - currentTime;
        currentTime = executionSlice.endTime;
        totalRunningTime += runningProcess.burstTime;

        // Process is complete
        if(runningProcess.remainingTime == 0)
        {
            runningProcess.endTime = currentTime;
            runningProcess.turnAroundTime = currentTime - runningProcess.arrivalTime;
            runningProcess.waitingTime = currentTime - runningProcess.arrivalTime - runningProcess.burstTime;

            totalWaitingTime += runningProcess.waitingTime;
            finishedProcessesCount++;
            terminatedProcesses.push_back(runningProcess);
        }

    }
}
