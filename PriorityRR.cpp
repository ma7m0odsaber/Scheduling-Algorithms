
#include "Scheduler.h"

// This function is used for sorting the processes by priority order
// Returns true if P1 < P2 (i.e. P1 priority is less than P2), and false otherwise
static bool comparePriority(PCB P1, PCB P2)
{

    if(P1.priority == P2.priority) return P1.arrivalTime < P2.arrivalTime;
    return P1.priority < P2.priority ;
}

// Priority Round-Robin Algorithm
void Scheduler::runPriorityRR(vector<PCB> processes, ull quanta)
{
    queue<PCB> readyQueue;
    PCB runningProcess;
    ull currentTime = 0;

    // sort based on arrival time
    sort(processes.begin(), processes.end(), comparePriority);

    // Add all processes sorted by arrival time to the ready queue
    for (auto p : processes)
        readyQueue.push(p);

    int Last_PID = readyQueue.back().PID;

    // Service the processes one by one
    while (!readyQueue.empty()) {

        // Remove the first-arrived process from the ready queue and schedule it for running
        runningProcess = readyQueue.front();
        readyQueue.pop();

        if(runningProcess.PID == Last_PID){
            vector<PCB> tmp;
            while(!readyQueue.empty()){
                tmp.push_back(readyQueue.front());
                readyQueue.pop();
            }
            sort(processes.begin(), processes.end(), comparePriority);
            for (auto p : tmp)
                readyQueue.push(p);
            Last_PID = readyQueue.back().PID;
        }

        // The process will run to completion as one slice
        ExecutionSlice executionSlice;
        executionSlice.PID = runningProcess.PID;
        executionSlice.length = runningProcess.burstTime;
        executionSlice.startTime = max(currentTime,runningProcess.arrivalTime);
        executionSlice.endTime = executionSlice.startTime + min(quanta, runningProcess.remainingTime);
        processingHistory.push_back(executionSlice);
        runningProcess.remainingTime -= min(quanta, runningProcess.remainingTime);

        cpuIdleTime += executionSlice.startTime - currentTime;
        currentTime = executionSlice.endTime;
        totalRunningTime += min(quanta, runningProcess.remainingTime);


        // Process is complete
        if (runningProcess.remainingTime == 0)
        {
            runningProcess.endTime = currentTime;
            runningProcess.turnAroundTime = currentTime - runningProcess.arrivalTime;
            runningProcess.waitingTime = currentTime - runningProcess.arrivalTime - runningProcess.burstTime;

            totalWaitingTime += runningProcess.waitingTime;
            finishedProcessesCount++;
            terminatedProcesses.push_back(runningProcess);
        }
        else readyQueue.push(runningProcess);

    }
}