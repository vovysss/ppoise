#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Process.h"
#include <vector>
#include <string>
#include <iostream>

namespace software {

class Scheduler {
private:
    std::string algorithm;
    int quantumMs;
    std::vector<Process> queue;
    int currentProcessIndex;

public:
    Scheduler(std::string schedulingAlgorithm, int timeQuantum)
        : algorithm(schedulingAlgorithm), quantumMs(timeQuantum),
          currentProcessIndex(0) {}

    void enqueue(const Process& process) {
        queue.push_back(process);
        std::cout << "Процесс " << process.getName() << " добавлен в очередь "
                  << algorithm << ".\n";
    }

    bool dequeue() {
        if (queue.empty()) {
            std::cout << "Очередь планировщика пуста.\n";
            return false;
        }
        std::cout << "Процесс " << queue.front().getName() << " снят с очереди.\n";
        queue.erase(queue.begin());
        return true;
    }

    void scheduleNext() {
        if (queue.empty()) {
            std::cout << "Нет процессов для планирования.\n";
            return;
        }
        currentProcessIndex = (currentProcessIndex + 1) % static_cast<int>(queue.size());
        std::cout << "Планировщик " << algorithm << " выбрал процесс "
                  << queue[currentProcessIndex].getName() << " (квант "
                  << quantumMs << " мс).\n";
    }

    void preempt() {
        std::cout << "Планировщик " << algorithm
                  << " вытеснил текущий процесс по истечении кванта.\n";
    }

    int calculatePriority(const Process& process) const {
        int priority = 100 - static_cast<int>(process.getCpuLoad());
        std::cout << "Приоритет процесса " << process.getName() << ": "
                  << priority << ".\n";
        return priority;
    }
};

} // namespace software

#endif
