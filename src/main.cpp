#include "queue.h"
std::vector<Process> processes;

int main(int argc, char **argv){

    //std::priority_queue<Process> process_queue;

    int loaded_lines;
    const std::string input_file_name = "sched1.in";
    std::queue<Process> FCFS_queue; 


    loaded_lines = readFromFile(input_file_name, &processes);
    printVectorOfProcesses(processes);
    std::cout << "\n";
    //std::cout << "Załadowane chwile czasowe: " <<loaded_lines << "\n";
    PriorityPlusSRTF(loaded_lines);

    

    

return 0;
}