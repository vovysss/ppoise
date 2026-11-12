#ifndef POWER_SUPPLY_UNIT_H
#define POWER_SUPPLY_UNIT_H

#include "Component.h"
#include <string>
#include <iostream>

namespace hardware {

class PowerSupplyUnit : public Component {
private:
    double maxWattage;
    std::string efficiencyRating; // Например, "80+ Gold"

public:
    PowerSupplyUnit(std::string id, std::string name, double power, double temp, 
                    double maxWatts, std::string efficiency)
        : Component(id, name, power, temp), maxWattage(maxWatts), efficiencyRating(efficiency) {}

    void powerOn() override {
        isWorking = true;
        std::cout << "Блок питания " << name << " поставляет энергию (Макс: " << maxWattage << "Вт, КПД: " << efficiencyRating << ").\n";
    }

    void powerOff() override {
        isWorking = false;
        std::cout << "Блок питания отключен. Система обесточена.\n";
    }

    // Проверка, хватит ли мощности для всей системы
    bool providePower(double requiredWatts) const {
        if (requiredWatts <= maxWattage) {
            std::cout << "Энергопотребление (" << requiredWatts << "Вт) в норме для БП.\n";
            return true;
        } else {
            std::cout << "ВНИМАНИЕ: Превышена максимальная мощность БП!\n";
            return false;
        }
    }
};

} // namespace hardware

#endif