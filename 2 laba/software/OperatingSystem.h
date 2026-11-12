#ifndef OPERATING_SYSTEM_H
#define OPERATING_SYSTEM_H

#include "Process.h"
#include <vector>
#include <string>
#include <iostream>

namespace software {

class OperatingSystem {
private:
    std::string name;
    std::string version;
    std::vector<Process> activeProcesses; // Композиция: ОС владеет процессами

public:
    OperatingSystem(std::string osName, std::string osVersion)
        : name(osName), version(osVersion) {}

    void boot() {
        std::cout << "Загрузка операционной системы " << name << " " << version << "...\n";
        std::cout << "Система готова к работе.\n";
    }

    void startProcess(int id, std::string procName, size_t memory, double cpuLoad) {
        activeProcesses.emplace_back(id, procName, memory, cpuLoad);
        activeProcesses.back().start();
    }

    void showTaskManger() {
        std::cout << "\n--- Диспетчер задач (" << name << ") ---\n";
        std::cout << "Активных процессов: " << activeProcesses.size() << "\n";
        for (const auto& proc : activeProcesses) {
            std::cout << " - PID: " << proc.getId() << " | Имя: " << proc.getName() 
                      << " | Нагрузка CPU: " << proc.getCpuLoad() << "%\n";
        }
        std::cout << "------------------------------------\n";
    }
};

} // namespace software

#endif