#include "queue.h"


const int Process::getPriority() const{
    return priority;
}

const int Process::getID() const{
    return id;
}

const int Process::getExecTime() const{
    return execTime;
}

const int Process::getStartTime() const{
    return startTime;
}

const bool Process::getStatus() const{
    return is_done;
}

const int Process::getExecutedTime() const{
    return executedTime;
}

const bool Process::getPrioritySRTF() const{
    return prioritySRTF;
}

void Process::setID(int _id){
    id = _id;
}

void Process::setExecTime(int _execTime){
    execTime = _execTime;
}

void Process::setPriority(int _priority){
    priority = _priority;
}

void Process::setStartTime(int _startTime){
    startTime = _startTime;
}

void Process::setStatus(bool _status){
    is_done = _status;
}

void Process::setExecutedTime(int _executedTime){
    executedTime = _executedTime;
}

void Process::setPrioritySRTF(){
    prioritySRTF = true;
}

void Process::increaseExecutedTime(){
    executedTime++;
}

const int Process::remainingTime() const{
    return execTime-executedTime;
}


bool operator<(const Process& process1, const Process& process2){
    if(!process1.getPrioritySRTF()){
        if(process1.getPriority() != process2.getPriority()){
        return process1.getPriority() > process2.getPriority();
        }else{
            return process1.getStartTime() > process2.getStartTime();
        }
    }else{
        if(process1.getPriority() != process2.getPriority()){
        return process1.getPriority() > process2.getPriority();
        }else{
            return process1.remainingTime() > process2.remainingTime();
        }
    }
}

bool operator == (const Process& process1, const Process& process2){
    return process1.getID() == process2.getID();
}

std::istream& operator >> (std::istream& in, Process& process){
    int id, execTime, priority;
    in >> id;
    in >> priority;
    in >> execTime;
    process.setID(id);
    process.setPriority(priority);
    process.setExecTime(execTime);
    return in;
}

bool CustomPriorityQueue::remove(const Process & value){
    auto it = std::find(this->c.begin(), this->c.end(), value);

    if(it == this->c.end()){
        return false;
    }
    if(it == this->c.begin()){
        this->pop();
    }else{
        this->c.erase(it);
        std::make_heap(this->c.begin(), this->c.end(), this->comp);
    }
    return true;

}

int readFromFile(const std::string input_file_name, std::vector<Process>* processes){
    //inicjalizacja zmiennych pomocniczych
    int loaded_lines = 0;
    std::string line;
    std::ifstream input_file(input_file_name);


    while(std::getline(input_file,line)){

        /*//pomijanie przypadków, gdy w danej sekundzie nie nadeszło żadne nowe zadanie
        space = 0;
        for(int i = 0; i < line.length(); i++){
            if(line[i] == ' ') space++;
        }*/

        //if((line.length() - space - 1) % 3 != 0 || (line.length() - 1 - space) == 0) continue; 

        //wczytywanie zadań do pamięci
        std::istringstream iss(line);
        int startTime = 0;
        iss >> startTime;
        while(iss.rdbuf()->in_avail() != 0){
            Process temp = Process(startTime);
            iss >> temp;
            processes->push_back(temp);
        }
        loaded_lines++;
}
    return loaded_lines;
}

void printVectorOfProcesses(const std::vector<Process> processes){
    for(int i = 0; i < processes.size(); i++){
        std::cout << "Czas rozpoczęcia: " << processes[i].getStartTime() << " ID: " << processes[i].getID() << " czas wykonania: " << processes[i].getExecTime() << " priorytet: " << processes[i].getPriority() << "\n";
    }
}

void fillFCFSQueue(const std::vector<Process> processes, std::queue<Process> *FCFS_queue, int startTime){
    for(int i = 0; i < processes.size(); i++){
        if(processes[i].getStartTime() == startTime) FCFS_queue->push(processes[i]);
    }
}

void fillRRQueue(const std::vector<Process> processes, std::queue<Process> *RR_queue, int startTime){
    fillFCFSQueue(processes, RR_queue, startTime);
}

void fillPriorityQueue(const std::vector<Process> processes, CustomPriorityQueue *priority_queue, int startTime){
    for(int i = 0; i < processes.size(); i++){
        if(processes[i].getStartTime() == startTime) priority_queue->push(processes[i]);
    }
}

void computeCurrentProcesses(const std::vector<Process> processes, std::vector<Process> *current_processes, int currentTime){
    for(int i = 0; i < processes.size(); i++){
        if(processes[i].getStartTime() == currentTime) current_processes->push_back(processes[i]);
    }
}

Process findShortestJob(const std::vector<Process> current_processes){
    int shortestExecTime = 10000;
    Process ShortestJob = Process(0);

    for(int i = 0; i < current_processes.size(); i++){
        if(current_processes[i].getExecTime() < shortestExecTime){
            ShortestJob = current_processes[i];
            shortestExecTime = ShortestJob.getExecTime();
        }
    }
    return ShortestJob;
}

Process findShortestJobSRTF(const std::vector<Process> current_processes){
    int shortestExecTime = 10000;
    Process ShortestJob = Process(0);

    for(int i = 0; i < current_processes.size(); i++){
        if(current_processes[i].remainingTime() < shortestExecTime){
            ShortestJob = current_processes[i];
            shortestExecTime = ShortestJob.remainingTime();
        }
    }
    return ShortestJob;
}

void FCFS(int totalTime){
    bool theEnd = false;
    int currentTime = 0;
    std::queue<Process> FCFS_queue; 
    Process Processor = Process(0);
    Processor.setStatus(true); //żeby pierwszy if się wykonał

    while(!theEnd){
        fillFCFSQueue(processes, &FCFS_queue, currentTime);

        if(Processor.getStatus() == true && !FCFS_queue.empty()){
             Processor = FCFS_queue.front();
             Processor.setStartTime(currentTime);
        }
        if(Processor.getStatus() == true && FCFS_queue.empty()){
            theEnd = true;
            Processor.setID(-1);
        }

        if(currentTime == (Processor.getExecTime()+ Processor.getStartTime()) -1){
            Processor.setStatus(true);
            FCFS_queue.pop();
        }

        std::cout << currentTime << " " << Processor.getID() << "\n";

        currentTime++;
    }
}

void SJF(int totalTime){
    bool theEnd = false;
    int currentTime = 0;
    std::vector<Process> current_processes;
    Process Processor = Process(0);
    Processor.setStatus(true); //żeby pierwszy if się wykonał

    while(!theEnd){
        computeCurrentProcesses(processes, &current_processes, currentTime);

        if(Processor.getStatus() == true && !current_processes.empty()){
             Processor = findShortestJob(current_processes);
             Processor.setStartTime(currentTime);
        }
        if(Processor.getStatus() == true && current_processes.empty()){
            theEnd = true;
            Processor.setID(-1);
        }

        if(currentTime == (Processor.getExecTime()+ Processor.getStartTime()) -1){

            Processor.setStatus(true);
            current_processes.erase(
                             std::remove_if(
                                current_processes.begin(), current_processes.end(), [&] (Process const & P){
                                    return P.getID() == Processor.getID();
                                }),
                                current_processes.end());

        }

        std::cout << currentTime << " " << Processor.getID() << "\n";

        currentTime++;
    }
}

void SRTF(int totalTime){
    bool theEnd = false;
    int currentTime = 0;
    std::vector<Process> current_processes;
    Process Processor = Process(0);
    Processor.setStatus(true); //żeby pierwszy if się wykonał

    while(!theEnd){
        if(currentTime <= totalTime) computeCurrentProcesses(processes, &current_processes, currentTime); //symulowanie krokowej pracy programu

        if(!current_processes.empty()){
            Process candidate = findShortestJobSRTF(current_processes);
            if(Processor.getStatus() == true){
                Processor = candidate;
            }else{
                if(candidate.remainingTime() < Processor.remainingTime()){
                    std::vector<Process>::iterator index = std::find_if(current_processes.begin(), current_processes.end(), [&](Process const & P){return P.getID() == Processor.getID();});
                    current_processes[std::distance(current_processes.begin(),index)].setExecutedTime(Processor.getExecutedTime());
                    Processor = candidate;
                }
            }
        }

        if(Processor.getStatus() == true && current_processes.empty()){
            theEnd = true;
            Processor.setID(-1);
        }

        if(Processor.getExecutedTime() == (Processor.getExecTime() - 1)){
            Processor.setStatus(true);
            current_processes.erase(std::remove_if(current_processes.begin(), current_processes.end(), [&](Process const & P){return P.getID() == Processor.getID();}));
        }


        std::cout << currentTime << " " << Processor.getID() << "\n";
        //if(currentTime == 25) break;
        Processor.increaseExecutedTime();
        currentTime++;
    }

}


void RR(int totalTime){
    bool theEnd = false;
    int currentTime = 1;
    std::queue<Process> RR_queue;
    Process Processor = Process(0);
    Processor.setStatus(true);
    fillRRQueue(processes, &RR_queue,0);
    Processor = RR_queue.front();
    Processor.increaseExecutedTime();
    std::cout << "0 " << Processor.getID() << "\n";
    while(!theEnd){
        if(currentTime <= totalTime) fillRRQueue(processes, &RR_queue, currentTime); //symulowanie pracy krokowej programu

        if(Processor.getExecutedTime() != (Processor.getExecTime())){
            RR_queue.push(Processor);
            RR_queue.pop();
            Processor = RR_queue.front();

        }else{
            RR_queue.pop();
            Processor = RR_queue.front();
        }

        if(RR_queue.empty()){
            theEnd = true;
            Processor.setID(-1);
        }



        std::cout << currentTime << " " << Processor.getID() << "\n";
        Processor.increaseExecutedTime();
        currentTime++;
        if(currentTime == 25) break;
    }
    
}

void PriorityPlusFCFS(int totalTime){
    bool theEnd = false;
    int currentTime = 0;
    CustomPriorityQueue priority_queue;
    Process Processor = Process(0);
    Processor.setStatus(true);

    while(!theEnd){
        fillPriorityQueue(processes, &priority_queue, currentTime);

        if(Processor.getStatus() == true && !priority_queue.empty()){
             Processor = priority_queue.top();
             Processor.setStartTime(currentTime);
        }
        if(Processor.getStatus() == true && priority_queue.empty()){
            theEnd = true;
            Processor.setID(-1);
        }

        if(currentTime == (Processor.getExecTime()+ Processor.getStartTime()) -1){
            Processor.setStatus(true);
            priority_queue.remove(Processor);
        }

        std::cout << currentTime << " " << Processor.getID() << "\n";

        currentTime++;

    }
}

void PriorityPlusFCFSW(int totalTime){
    bool theEnd = false;
    int currentTime = 0;
    CustomPriorityQueue priority_queue;
    Process Processor = Process(0);
    Processor.setStatus(true);

    while(!theEnd){
        fillPriorityQueue(processes, &priority_queue, currentTime);

        if(!priority_queue.empty()){
            Process candidate = priority_queue.top();
            if(Processor.getStatus() == true){
                Processor = candidate;
            }else{
                if(candidate.getPriority() < Processor.getPriority()){
                    priority_queue.remove(Processor);
                    priority_queue.push(Processor);
                    Processor = candidate;
                }
            }
        }

        if(Processor.getStatus() == true && priority_queue.empty()){
            theEnd = true;
            Processor.setID(-1);
        }

        if(Processor.getExecutedTime() == (Processor.getExecTime() - 1)){
            Processor.setStatus(true);
            priority_queue.remove(Processor);
        }


        std::cout << currentTime << " " << Processor.getID() << "\n";
        //if(currentTime == 25) break;
        Processor.increaseExecutedTime();



        currentTime++;
    }



}

void PriorityPlusSRTF(int TotalTime){

        bool theEnd = false;
    int currentTime = 0;
    CustomPriorityQueue priority_queue;
    Process Processor = Process(0);
    Processor.setStatus(true);

    while(!theEnd){
        fillPriorityQueue(processes, &priority_queue, currentTime);

        if(!priority_queue.empty()){
            Process candidate = priority_queue.top();
            if(Processor.getStatus() == true){
                Processor = candidate;
            }else{
                if(candidate.getPriority() < Processor.getPriority()){
                    priority_queue.remove(Processor);
                    priority_queue.push(Processor);
                    Processor = candidate;
                }
            }
        }

        if(Processor.getStatus() == true && priority_queue.empty()){
            theEnd = true;
            Processor.setID(-1);
        }

        if(Processor.getExecutedTime() == (Processor.getExecTime() - 1)){
            Processor.setStatus(true);
            priority_queue.remove(Processor);
        }


        std::cout << currentTime << " " << Processor.getID() << "\n";
        Processor.increaseExecutedTime();



        currentTime++;

}
}
