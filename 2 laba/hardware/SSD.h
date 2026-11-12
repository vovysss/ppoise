#ifndef SSD_H
#define SSD_H

#include "StorageDevice.h"
#include <iostream>

namespace hardware {

class SSD : public StorageDevice {
private:
    std::string nandType; // TLC, QLC, MLC
    size_t TBW;           // Total Bytes Written (ресурс перезаписи)

public:
    SSD(std::string id, std::string name, double power, double temp, 
        size_t capacity, double readSpeed, double writeSpeed, std::string nand, size_t tbwLimit)
        : StorageDevice(id, name, power, temp, capacity, readSpeed, writeSpeed), 
          nandType(nand), TBW(tbwLimit) {}

    void powerOn() override {
        isWorking = true;
        std::cout << "SSD " << name << " (" << nandType << ") инициализирован мгновенно.\n";
    }

    void powerOff() override {
        isWorking = false;
        std::cout << "SSD " << name << " обесточен.\n";
    }

    void trim() {
        std::cout << "Выполнение команды TRIM для оптимизации ячеек памяти SSD "
                  << name << " (ресурс TBW: " << TBW << " ТБ).\n";
    }
};

} // namespace hardware

#endif