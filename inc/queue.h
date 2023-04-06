#ifndef QUEUE_HH
#define QUEUE_H
#include <queue>
#include <iostream>
#include "fstream"
#include "sstream"
#include "string"
#include <algorithm>

class Process {
    private:
    int id, execTime, priority, startTime, executedTime = 0;
    bool is_done = false;
    bool prioritySRTF = false;
    public:
    //konstruktory
    Process(int _id, int _execTime,int _priority,int _startTime)
    :id(_id), execTime(_execTime), priority(_priority), startTime(_startTime){};
    Process(int _startTime)
    :id(0), execTime(0), priority(0), startTime(_startTime){};


    //funkcje get
    const int getPriority() const;
    const int getID() const;
    const int getExecTime() const;
    const int getStartTime() const;
    const bool getStatus() const;
    const int getExecutedTime() const;
    const bool getPrioritySRTF() const;
    //funkcje set
    void setID(int _id);
    void setExecTime(int _execTime);
    void setPriority(int _priority);
    void setStartTime(int _startTime);
    void setStatus(bool _status);
    void setExecutedTime(int _executedTime);
    void setPrioritySRTF();

    void increaseExecutedTime();
    const int remainingTime() const;
};

class CustomPriorityQueue : public std::priority_queue<Process, std::vector<Process>>{
public:
    bool remove(const Process & value);
};

extern std::vector<Process> processes;


bool operator < (const Process& process1, const Process& process2); // porównanie potrzebne do kolejki priorytetowej
bool operator == (const Process& process1, const Process& process2);
std::istream& operator >> (std::istream& in, Process& process); // łatwe wczytywanie z linii

int readFromFile(const std::string input_file_name, std::vector<Process>* processes);
void printVectorOfProcesses(const std::vector<Process> processes);
void fillFCFSQueue(const std::vector<Process> processes, std::queue<Process> *FCFS_queue, int startTime);
void fillRRQueue(const std::vector<Process> processes, std::queue<Process> *RR_queue, int startTime);
void fillPriorityQueue(const std::vector<Process> processes, CustomPriorityQueue *priority_queue, int startTime);
void computeCurrentProcesses();
Process findShortestJob(const std::vector<Process> current_processes);
Process findShortestJobSRTF(const std::vector<Process> current_processes);
void FCFS(int totalTime);
void SJF(int totalTime);
void SRTF(int totalTime);
void RR(int totalTime);
void PriorityPlusFCFSW(int totalTime);
void PriorityPlusSRTF(int TotalTime);
void PriorityPlusFCFS(int totalTime);

#endif