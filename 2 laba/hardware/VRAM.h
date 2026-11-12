#ifndef VRAM_H
#define VRAM_H

#include "Component.h"
#include <string>
#include <iostream>

namespace hardware {

class VRAM : public Component {
private:
    size_t capacityGB;
    double bandwidthGBs; // Пропускная способность
    std::string memoryType; // Например, GDDR6X

public:
    VRAM(std::string id, std::string name, double power, double temp, 
         size_t capacity, double bandwidth, std::string type)
        : Component(id, name, power, temp), capacityGB(capacity), 
          bandwidthGBs(bandwidth), memoryType(type) {}

    void powerOn() override {
        isWorking = true;
        std::cout << "Видеопамять " << name << " (" << capacityGB << "GB " << memoryType << ", пропускная способность: " << bandwidthGBs << " ГБ/с) активна.\n";
    }

    void powerOff() override {
        isWorking = false;
        std::cout << "Видеопамять " << name << " обесточена.\n";
    }

    size_t getCapacity() const { return capacityGB; }
    double getBandwidth() const { return bandwidthGBs; }
};

} // namespace hardware

#endif