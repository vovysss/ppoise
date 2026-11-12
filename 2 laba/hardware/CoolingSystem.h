#ifndef COOLING_SYSTEM_H
#define COOLING_SYSTEM_H

#include "Component.h"
#include "Fan.h"
#include <vector>
#include <string>
#include <iostream>

namespace hardware {

class CoolingSystem : public Component {
private:
    std::string coolingType; // "Air" (воздушное) или "Liquid" (жидкостное)
    std::vector<Fan> fans;   // Композиция: система охлаждения владеет вентиляторами
    double coolingEfficiency; // Коэффициент эффективности

public:
    CoolingSystem(std::string id, std::string name, double power, double temp, 
                  std::string type, double efficiency, int fanCount, double fanMaxRpm, double fanDiameter)
        : Component(id, name, power, temp), coolingType(type), coolingEfficiency(efficiency) {
        
        // Создаем вентиляторы внутри конструктора системы охлаждения
        for (int i = 1; i <= fanCount; ++i) {
            fans.emplace_back(id + "-Fan" + std::to_string(i), name + " Fan " + std::to_string(i), 3.0, 25.0, fanMaxRpm, fanDiameter);
        }
    }

    void powerOn() override {
        isWorking = true;
        std::cout << "Система охлаждения " << name << " (" << coolingType << ") активирована.\n";
        for (auto& fan : fans) {
            fan.powerOn();
        }
    }

    void powerOff() override {
        for (auto& fan : fans) {
            fan.powerOff();
        }
        isWorking = false;
        std::cout << "Система охлаждения " << name << " выключена.\n";
    }

    // Содержательный метод рассеивания тепла
    double dissipateHeat(double currentTemp, double heatGenerated) {
        double reducedTemp = currentTemp + heatGenerated - (coolingEfficiency * 0.2);
        if (reducedTemp < 25.0) reducedTemp = 25.0; // Ниже комнатной температуры упасть не может
        std::cout << "Система охлаждения " << name << " рассеивает тепло. Итоговая температура: " << reducedTemp << "°C.\n";
        return reducedTemp;
    }
};

} // namespace hardware

#endif