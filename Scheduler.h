#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include <queue>
#include "algorithm"
#include <iostream>
#include <iomanip>

using namespace std;

#define TIME_QUANTUM	4
typedef unsigned long long ull;

enum Scheduling_Algorithm
{
    FCFS,
    SJF,
    PRIORITY,
    RR,
    PRIORITY_RR
};

// This records a process's activity in the CPU
// This is used to generate the execution history (Gantt chart)
typedef struct ExecutionSlice
{
    ull PID;
    ull length;
    ull startTime;
    ull endTime;
} ExecutionSlice;

// Process control block
struct PCB
{
    ull PID;
    ull arrivalTime;
    ull endTime;
    ull burstTime;
    ull priority;
    ull waitingTime;
    ull turnAroundTime;
    ull remainingTime;
};

class Scheduler
{
private:
	vector<PCB> createdProcesses;	// Contains initial data
	vector<PCB> terminatedProcesses;	// Contains execution info

	unsigned int totalProcessesCount;
	unsigned int cpuIdleTime;
	unsigned int finishedProcessesCount;
	unsigned int totalWaitingTime;
	unsigned int totalRunningTime;

	Scheduling_Algorithm schedulingAlgorithm;
	vector<ExecutionSlice> processingHistory;

public:
	Scheduler();	// Constructor
	void run();
	void createProcess(ull arrivalTime, ull burstTime, ull priority);
	void printExecutionHistory();
	void printStatistics();
	void printProcesses();
	void setSchedulingAlgorithm(Scheduling_Algorithm schedAlgorithm);
	void runFCFS(vector<PCB> processes);
	void runSJF(vector<PCB> processes);
	void runPriority(vector<PCB> processes);
	void runRR(vector<PCB> processes, ull quanta);
    void runPriorityRR(vector<PCB> processes, ull quanta);
};

#endif /* SCHEDULER_H */
