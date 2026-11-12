#ifndef STORAGE_DEVICE_H
#define STORAGE_DEVICE_H

#include "Component.h"
#include <string>
#include <iostream>

namespace hardware {

class StorageDevice : public Component {
protected:
    size_t capacityGB;
    double readSpeedMBs;
    double writeSpeedMBs;
    size_t usedSpaceGB;

public:
    StorageDevice(std::string id, std::string name, double power, double temp, 
                  size_t capacity, double readSpeed, double writeSpeed)
        : Component(id, name, power, temp), capacityGB(capacity), 
          readSpeedMBs(readSpeed), writeSpeedMBs(writeSpeed), usedSpaceGB(0) {}

    virtual void readData(size_t sizeMB) {
        std::cout << "Чтение " << sizeMB << " МБ с накопителя " << name << " на скорости " << readSpeedMBs << " МБ/с.\n";
    }

    virtual void writeData(size_t sizeMB) {
        if (usedSpaceGB + sizeMB > capacityGB) {
            std::cout << "Ошибка: недостаточно места на накопителе " << name << "!\n";
            return;
        }
        usedSpaceGB += sizeMB;
        std::cout << "Запись " << sizeMB << " МБ на накопитель " << name << " на скорости " << writeSpeedMBs << " МБ/с.\n";
    }

    size_t getFreeSpace() const { return capacityGB - usedSpaceGB; }
    size_t getCapacity() const { return capacityGB; }
};

} // namespace hardware

#endif