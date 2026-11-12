#ifndef ROM_MODULE_H
#define ROM_MODULE_H

#include "Component.h"
#include <string>
#include <iostream>

namespace hardware {

class ROMModule : public Component {
private:
    size_t capacityKB;
    std::string firmwareVersion; // Версия BIOS / UEFI

public:
    ROMModule(std::string id, std::string name, double power, double temp, 
              size_t capacity, std::string firmware)
        : Component(id, name, power, temp), capacityKB(capacity), firmwareVersion(firmware) {}

    void powerOn() override {
        isWorking = true;
        std::cout << "ПЗУ / UEFI микросхема " << name << " (Прошивка: " << firmwareVersion << ", " << capacityKB << " КБ) инициализирована.\n";
    }

    void powerOff() override {
        isWorking = false;
        std::cout << "ПЗУ " << name << " обесточено.\n";
    }

    void flashFirmware(std::string newVersion) {
        firmwareVersion = newVersion;
        std::cout << "Обновление микропрограммы ROM до версии " << firmwareVersion << ".\n";
    }

    std::string getFirmwareVersion() const { return firmwareVersion; }
};

} // namespace hardware

#endif