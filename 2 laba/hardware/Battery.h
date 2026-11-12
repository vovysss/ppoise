#ifndef BATTERY_H
#define BATTERY_H

#include "Component.h"
#include <string>
#include <iostream>

namespace hardware {

class Battery : public Component {
private:
    int capacityMah;
    int chargePercent;
    int cycleCount;
    bool isCharging;
    Component* poweredDevice;

public:
    Battery(std::string id, std::string name, double power, double temp,
            int capacity, Component* device = nullptr)
        : Component(id, name, power, temp), capacityMah(capacity),
          chargePercent(100), cycleCount(0), isCharging(false),
          poweredDevice(device) {}

    void powerOn() override {
        isWorking = true;
        if (poweredDevice) {
            poweredDevice->powerOn();
        }
        std::cout << "Батарея " << name << " (" << capacityMah << " мАч) подает питание.\n";
    }

    void powerOff() override {
        if (poweredDevice) {
            poweredDevice->powerOff();
        }
        isWorking = false;
        std::cout << "Батарея " << name << " отключена.\n";
    }

    void charge(int percent) {
        isCharging = true;
        chargePercent += percent;
        if (chargePercent > 100) chargePercent = 100;
        ++cycleCount;
        std::cout << "Батарея " << name << " заряжена до " << chargePercent
                  << "% (циклов: " << cycleCount << ").\n";
    }

    void discharge(int percent) {
        chargePercent -= percent;
        if (chargePercent < 0) chargePercent = 0;
        std::cout << "Батарея " << name << " разряжена до " << chargePercent << "%.\n";
    }

    void reportHealth() const {
        std::string state = chargePercent > 70 ? "отличное" :
                            chargePercent > 30 ? "удовлетворительное" : "критическое";
        std::cout << "Состояние батареи " << name << ": " << state
                  << " (" << chargePercent << "%, циклов " << cycleCount << ").\n";
    }
};

} // namespace hardware

#endif
