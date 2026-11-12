#ifndef GPU_CORE_H
#define GPU_CORE_H

#include <string>
#include <iostream>

namespace hardware {

class GPUCore {
private:
    int coreId;
    double frequencyMHz;
    double currentLoadPercent;
    bool isActive;

public:
    GPUCore(int id, double freq) 
        : coreId(id), frequencyMHz(freq), currentLoadPercent(0.0), isActive(false) {}

    void activate() {
        isActive = true;
        std::cout << "Графическое ядро #" << coreId << " (Частота: " << frequencyMHz << " МГц) активировано.\n";
    }

    void deactivate() {
        isActive = false;
        currentLoadPercent = 0.0;
        std::cout << "Графическое ядро #" << coreId << " отключено.\n";
    }

    void setLoad(double load) {
        currentLoadPercent = load;
        std::cout << "Нагрузка GPU ядра #" << coreId << " установлена на уровне " << currentLoadPercent << "%.\n";
    }

    int getId() const { return coreId; }
    double getFrequency() const { return frequencyMHz; }
    double getLoad() const { return currentLoadPercent; }
};

} // namespace hardware

#endif