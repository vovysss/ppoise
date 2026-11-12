#ifndef ABSTRACT_PROCESSOR_H
#define ABSTRACT_PROCESSOR_H

#include "Component.h"
#include <string>

namespace hardware {

class AbstractProcessor : public Component {
protected:
    int coreCount;
    double clockFrequencyGHz;
    int socketType;

public:
    AbstractProcessor(std::string id, std::string name, double power, double temp, 
                      int cores, double clock, int socket)
        : Component(id, name, power, temp), coreCount(cores), 
          clockFrequencyGHz(clock), socketType(socket) {}

    // Чисто виртуальные методы для реализации в наследниках (полиморфизм)
    virtual void executeInstructions() = 0;
    virtual void overclock(double percent) = 0;

    // Геттеры
    int getCoreCount() const { return coreCount; }
    double getClockFrequency() const { return clockFrequencyGHz; }
    int getSocketType() const { return socketType; }
};

} // namespace hardware

#endif
