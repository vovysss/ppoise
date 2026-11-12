#ifndef BLUETOOTH_ADAPTER_H
#define BLUETOOTH_ADAPTER_H

#include "Component.h"
#include "../software/File.h"
#include <string>
#include <iostream>

namespace hardware {

class BluetoothAdapter : public Component {
private:
    std::string bluetoothVersion;
    int pairedDeviceCount;
    bool isEnabled;

public:
    BluetoothAdapter(std::string id, std::string name, double power, double temp,
                     std::string version)
        : Component(id, name, power, temp), bluetoothVersion(version),
          pairedDeviceCount(0), isEnabled(false) {}

    void powerOn() override {
        isWorking = true;
        isEnabled = true;
        std::cout << "Bluetooth " << bluetoothVersion << " адаптер " << name
                  << " включен.\n";
    }

    void powerOff() override {
        isEnabled = false;
        isWorking = false;
        std::cout << "Bluetooth адаптер " << name << " выключен.\n";
    }

    void pairDevice(const std::string& deviceName) {
        if (!isEnabled) {
            std::cout << "Bluetooth адаптер " << name << " выключен.\n";
            return;
        }
        ++pairedDeviceCount;
        std::cout << "Устройство " << deviceName << " сопряжено с "
                  << name << ". Всего сопряжено: " << pairedDeviceCount << ".\n";
    }

    void sendFile(const software::File& file) {
        std::cout << "Bluetooth адаптер " << name << " передает файл "
                  << file.getName() << " (" << file.getSize() << " МБ).\n";
    }

    void disconnectDevice(const std::string& deviceName) {
        if (pairedDeviceCount > 0) --pairedDeviceCount;
        std::cout << "Устройство " << deviceName << " отключено от Bluetooth адаптера "
                  << name << ".\n";
    }
};

} // namespace hardware

#endif
