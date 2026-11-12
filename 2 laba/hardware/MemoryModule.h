#ifndef MEMORY_MODULE_H
#define MEMORY_MODULE_H

#include "Component.h"
#include <string>

namespace hardware {

class MemoryModule : public Component {
protected:
    size_t capacityGB;
    double frequencyMHz;

public:
    MemoryModule(std::string id, std::string name, double power, double temp, 
                 size_t capacity, double freq)
        : Component(id, name, power, temp), capacityGB(capacity), frequencyMHz(freq) {}

    // Чисто виртуальный метод самодиагностики памяти
    virtual void selfTest() = 0;

    size_t getCapacity() const { return capacityGB; }
    double getFrequency() const { return frequencyMHz; }
};

} // namespace hardware

#endif