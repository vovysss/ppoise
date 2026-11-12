#ifndef CORE_H
#define CORE_H

#include <iostream>

namespace hardware {

class Core {
private:
    int coreId;
    double currentLoad; // в процентах (0 - 100)
    double frequency;   // в ГГц

public:
    Core(int id, double freq) : coreId(id), currentLoad(0.0), frequency(freq) {}

    void executeInstruction() {
        std::cout << "Ядро #" << coreId << " выполняет инструкцию на частоте " << frequency << " ГГц.\n";
    }

    void setLoad(double load) {
        currentLoad = load;
    }

    double getCurrentLoad() const { return currentLoad; }
};

} // namespace hardware

#endif