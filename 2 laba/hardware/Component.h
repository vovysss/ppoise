#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

namespace hardware {

class Component {
protected:
    std::string id;
    std::string name;
    double powerConsumption; // в Ваттах
    double temperature;      // в градусах Цельсия
    bool isWorking;

public:
    Component(std::string id, std::string name, double power, double temp)
        : id(id), name(name), powerConsumption(power), temperature(temp), isWorking(false) {}

    virtual ~Component() = default;

    // Чисто виртуальные методы — наследники обязаны их реализовать
    virtual void powerOn() = 0;
    virtual void powerOff() = 0;

    // Полезные методы (поведение)
    double getTemperature() const { return temperature; }
    double getPowerConsumption() const { return powerConsumption; }
    bool checkHealth() const { return temperature < 90.0 && isWorking; }
};

} // namespace hardware

#endif