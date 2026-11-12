#ifndef HDD_H
#define HDD_H

#include "StorageDevice.h"
#include <iostream>

namespace hardware {

class HDD : public StorageDevice {
private:
    int rpm; // Скорость вращения шпинделя (например, 7200 RPM)
    double noiseLevelDb;

public:
    HDD(std::string id, std::string name, double power, double temp, 
        size_t capacity, double readSpeed, double writeSpeed, int spindleSpeed, double noise)
        : StorageDevice(id, name, power, temp, capacity, readSpeed, writeSpeed), 
          rpm(spindleSpeed), noiseLevelDb(noise) {}

    void powerOn() override {
        isWorking = true;
        std::cout << "HDD " << name << " раскручивает шпиндель до " << rpm
                  << " об/мин (шум " << noiseLevelDb << " дБ).\n";
    }

    void powerOff() override {
        isWorking = false;
        std::cout << "HDD " << name << " паркует головки и выключается.\n";
    }

    void defragment() {
        std::cout << "Запущена дефрагментация механического диска " << name << "...\n";
    }
};

} // namespace hardware

#endif