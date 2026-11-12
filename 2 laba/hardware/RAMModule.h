#ifndef RAM_MODULE_H
#define RAM_MODULE_H

#include "Component.h"
#include <string>
#include <iostream>

namespace hardware {

class RAMModule : public Component {
private:
    size_t capacityGB;
    double speedMHz;
    std::string memoryType; // DDR4 / DDR5

public:
    RAMModule(std::string id, std::string name, double power, double temp, 
              size_t capacity, double speed, std::string type)
        : Component(id, name, power, temp), capacityGB(capacity), 
          speedMHz(speed), memoryType(type) {}

    void powerOn() override {
        isWorking = true;
        std::cout << "ОЗУ " << name << " (" << capacityGB << "GB, " << memoryType
                  << ", " << speedMHz << " МГц) активирована.\n";
    }

    void powerOff() override {
        isWorking = false;
        std::cout << "ОЗУ " << name << " обесточена.\n";
    }

    // Содержательный метод
    void allocateMemory(size_t sizeMB) {
        std::cout << "Выделено " << sizeMB << " МБ в модуле памяти " << name << ".\n";
    }
};

} // namespace hardware

#endif