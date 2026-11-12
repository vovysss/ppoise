#ifndef KERNEL_H
#define KERNEL_H

#include "Driver.h"
#include "Process.h"
#include <vector>
#include <string>
#include <iostream>

namespace software {

class Kernel {
private:
    std::string version;
    std::vector<Driver> modules;
    bool isRunning;

public:
    Kernel(std::string kernelVersion)
        : version(kernelVersion), isRunning(false) {}

    void boot() {
        isRunning = true;
        std::cout << "Ядро " << version << " загружено. Инициализация подсистем...\n";
    }

    void shutdown() {
        isRunning = false;
        std::cout << "Ядро " << version << " завершает работу.\n";
    }

    void loadModule(const Driver& driver) {
        modules.push_back(driver);
        std::cout << "Ядро " << version << " загрузило модуль "
                  << driver.getDriverName() << ".\n";
    }

    bool unloadModule(const std::string& moduleName) {
        for (auto it = modules.begin(); it != modules.end(); ++it) {
            if (it->getDriverName() == moduleName) {
                modules.erase(it);
                std::cout << "Модуль " << moduleName << " выгружен из ядра.\n";
                return true;
            }
        }
        std::cout << "Модуль " << moduleName << " не найден.\n";
        return false;
    }

    void scheduleProcess(Process& process) {
        if (!isRunning) {
            std::cout << "Ядро не запущено, планирование невозможно.\n";
            return;
        }
        std::cout << "Ядро " << version << " поставило процесс "
                  << process.getName() << " в очередь планировщика.\n";
    }
};

} // namespace software

#endif
