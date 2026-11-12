#ifndef FAN_H
#define FAN_H

#include "Component.h"
#include <string>
#include <iostream>

namespace hardware {

class Fan : public Component {
private:
    double maxRPM;
    double currentRPM;
    double diameterMM;

public:
    Fan(std::string id, std::string name, double power, double temp, 
        double maxRpmVal, double diameter)
        : Component(id, name, power, temp), maxRPM(maxRpmVal), 
          currentRPM(0.0), diameterMM(diameter) {}

    void powerOn() override {
        isWorking = true;
        currentRPM = maxRPM * 0.3; // Старт на 30% оборотов
        std::cout << "Вентилятор " << name << " (" << diameterMM << " мм) запущен. Обороты: " << currentRPM << " об/мин.\n";
    }

    void powerOff() override {
        isWorking = false;
        currentRPM = 0.0;
        std::cout << "Вентилятор " << name << " остановлен.\n";
    }

    void setSpeed(double rpm) {
        if (rpm > maxRPM) rpm = maxRPM;
        currentRPM = rpm;
        std::cout << "Скорость вентилятора " << name << " изменена на " << currentRPM << " об/мин.\n";
    }

    double getCurrentRPM() const { return currentRPM; }
};

} // namespace hardware

#endif