#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include "Component.h"
#include "../exceptions/OverheatingException.h"
#include <string>
#include <iostream>

namespace hardware {

class TemperatureSensor : public Component {
private:
    double currentTemp;
    double warningThreshold;
    double criticalThreshold;
    Component* monitoredComponent;

public:
    TemperatureSensor(std::string id, std::string name, double power, double temp,
                      double warning, double critical, Component* monitored = nullptr)
        : Component(id, name, power, temp), currentTemp(temp),
          warningThreshold(warning), criticalThreshold(critical),
          monitoredComponent(monitored) {}

    void powerOn() override {
        isWorking = true;
        std::cout << "Датчик температуры " << name << " начал мониторинг.\n";
    }

    void powerOff() override {
        isWorking = false;
        std::cout << "Датчик температуры " << name << " остановлен.\n";
    }

    double readTemperature() const {
        return currentTemp;
    }

    void triggerAlarm() {
        if (currentTemp >= criticalThreshold) {
            std::string target = monitoredComponent ? name : "компонент";
            throw exceptions::OverheatingException(target, currentTemp);
        }
        if (currentTemp >= warningThreshold) {
            std::cout << "ПРЕДУПРЕЖДЕНИЕ: температура " << currentTemp
                      << "°C близка к критической.\n";
        } else {
            std::cout << "Температура " << currentTemp << "°C в норме.\n";
        }
    }
};

} // namespace hardware

#endif
