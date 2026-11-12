#ifndef CHIPSET_H
#define CHIPSET_H

#include <string>
#include <iostream>

namespace hardware {

class Chipset {
private:
    std::string chipsetModel;
    int pcieLanesCount;
    bool supportsOverclocking;
    bool isActive;

public:
    Chipset(std::string model, int pcieLanes, bool overclock)
        : chipsetModel(model), pcieLanesCount(pcieLanes), 
          supportsOverclocking(overclock), isActive(false) {}

    void initialize() {
        isActive = true;
        std::cout << "Чипсет " << chipsetModel << " инициализирован (PCIe линий: " << pcieLanesCount << ").\n";
    }

    void shutdown() {
        isActive = false;
        std::cout << "Чипсет " << chipsetModel << " выключен.\n";
    }

    std::string getModel() const { return chipsetModel; }
    bool isOverclockSupported() const { return supportsOverclocking; }
};

} // namespace hardware

#endif