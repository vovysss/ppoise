#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include "../hardware/Component.h"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

namespace software {

class DeviceManager {
private:
    std::vector<hardware::Component*> devices;
    std::vector<std::string> conflicts;
    bool isScanning;

public:
    DeviceManager() : isScanning(false) {}

    void registerDevice(hardware::Component* device) {
        if (device == nullptr) {
            std::cout << "Нельзя зарегистрировать пустое устройство.\n";
            return;
        }
        devices.push_back(device);
        std::cout << "Устройство зарегистрировано. Всего устройств: "
                  << devices.size() << ".\n";
    }

    bool detectDevice(const std::string& deviceName) {
        isScanning = true;
        for (auto* device : devices) {
            if (device->checkHealth()) {
                std::cout << "Устройство " << deviceName
                          << " обнаружено и исправно.\n";
                isScanning = false;
                return true;
            }
        }
        isScanning = false;
        std::cout << "Устройство " << deviceName << " не обнаружено.\n";
        return false;
    }

    void resolveConflict(const std::string& conflictName) {
        auto it = std::find(conflicts.begin(), conflicts.end(), conflictName);
        if (it != conflicts.end()) {
            conflicts.erase(it);
            std::cout << "Конфликт " << conflictName << " разрешен.\n";
        } else {
            std::cout << "Конфликт " << conflictName << " не найден.\n";
        }
    }

    void scanDevices() {
        isScanning = true;
        std::cout << "Сканирование шин и устройств... найдено "
                  << devices.size() << " устройств.\n";
        isScanning = false;
    }
};

} // namespace software

#endif
