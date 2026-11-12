#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include "OperatingSystem.h"
#include <string>
#include <iostream>

namespace software {

class VirtualMachine {
private:
    std::string vmId;
    int allocatedRamGB;
    int allocatedCpuCores;
    bool isRunning;
    OperatingSystem* guestOS;

public:
    VirtualMachine(std::string id, int ramGB, int cpuCores, OperatingSystem* os)
        : vmId(id), allocatedRamGB(ramGB), allocatedCpuCores(cpuCores),
          isRunning(false), guestOS(os) {}

    void startVM() {
        if (guestOS == nullptr) {
            std::cout << "Гостевая ОС не задана для ВМ " << vmId << ".\n";
            return;
        }
        isRunning = true;
        guestOS->boot();
        std::cout << "Виртуальная машина " << vmId << " запущена.\n";
    }

    void stopVM() {
        isRunning = false;
        std::cout << "Виртуальная машина " << vmId << " остановлена.\n";
    }

    void snapshot() const {
        std::cout << "Снимок состояния ВМ " << vmId << " создан.\n";
    }

    void restoreSnapshot() {
        std::cout << "ВМ " << vmId << " восстановлена из снимка.\n";
    }

    void allocateResources(int ramGB, int cpuCores) {
        allocatedRamGB = ramGB;
        allocatedCpuCores = cpuCores;
        std::cout << "ВМ " << vmId << " получила " << allocatedRamGB
                  << " ГБ RAM и " << allocatedCpuCores << " ядер CPU.\n";
    }
};

} // namespace software

#endif
