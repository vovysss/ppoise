#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <iostream>

namespace software {

class Process {
private:
    int processId;
    std::string name;
    size_t memoryRequiredMB;
    double cpuLoadPercent;
    bool isRunning;

public:
    Process(int id, std::string procName, size_t memory, double cpuLoad)
        : processId(id), name(procName), memoryRequiredMB(memory), 
          cpuLoadPercent(cpuLoad), isRunning(false) {}

    void start() {
        isRunning = true;
        std::cout << "Процесс [" << name << "] (PID: " << processId << ") запущен. Требуется памяти: " << memoryRequiredMB << " MB.\n";
    }

    void terminate() {
        isRunning = false;
        std::cout << "Процесс [" << name << "] (PID: " << processId << ") завершен.\n";
    }

    int getId() const { return processId; }
    std::string getName() const { return name; }
    double getCpuLoad() const { return cpuLoadPercent; }
};

} // namespace software

#endif