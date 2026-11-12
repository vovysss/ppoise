#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "../hardware/RAMModule.h"
#include <iostream>

namespace software {

class MemoryManager {
private:
    int totalMemoryGB;
    int allocatedMemoryGB;
    int pageSizeKB;
    hardware::RAMModule* ram;

public:
    MemoryManager(int totalMemory, int pageSize, hardware::RAMModule* ramModule)
        : totalMemoryGB(totalMemory), allocatedMemoryGB(0),
          pageSizeKB(pageSize), ram(ramModule) {}

    bool allocatePages(int pageCount) {
        int requestedGB = (pageCount * pageSizeKB) / (1024 * 1024);
        if (requestedGB == 0) requestedGB = 1;
        if (allocatedMemoryGB + requestedGB > totalMemoryGB) {
            std::cout << "Недостаточно памяти: запрошено " << requestedGB
                      << " ГБ, свободно " << (totalMemoryGB - allocatedMemoryGB)
                      << " ГБ.\n";
            return false;
        }
        allocatedMemoryGB += requestedGB;
        if (ram) {
            ram->allocateMemory(pageCount * pageSizeKB / 1024);
        }
        std::cout << "Выделено " << requestedGB << " ГБ (" << pageCount
                  << " страниц). Использовано " << allocatedMemoryGB << "/"
                  << totalMemoryGB << " ГБ.\n";
        return true;
    }

    bool freePages(int pageCount) {
        int releasedGB = (pageCount * pageSizeKB) / (1024 * 1024);
        if (releasedGB == 0) releasedGB = 1;
        allocatedMemoryGB -= releasedGB;
        if (allocatedMemoryGB < 0) allocatedMemoryGB = 0;
        std::cout << "Освобождено " << pageCount << " страниц. Использовано "
                  << allocatedMemoryGB << "/" << totalMemoryGB << " ГБ.\n";
        return true;
    }

    void defragment() {
        std::cout << "Менеджер памяти выполняет дефрагментацию страниц.\n";
    }

    double checkFragmentation() const {
        double fragmentation = totalMemoryGB == 0 ? 0.0 :
            (static_cast<double>(allocatedMemoryGB) / totalMemoryGB) * 100.0;
        std::cout << "Фрагментация памяти: " << fragmentation << "%.\n";
        return fragmentation;
    }

    void swapOut(int pages) {
        std::cout << "Менеджер памяти выгрузил " << pages
                  << " страниц в файл подкачки.\n";
    }
};

} // namespace software

#endif
