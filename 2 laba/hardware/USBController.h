#ifndef USB_CONTROLLER_H
#define USB_CONTROLLER_H

#include "Component.h"
#include <vector>
#include <string>
#include <iostream>

namespace hardware {

class USBController : public Component {
private:
    int portCount;
    std::string usbStandard;
    double powerPerPortWatts;
    std::vector<Component*> connectedDevices;

public:
    USBController(std::string id, std::string name, double power, double temp,
                  int ports, std::string standard, double portPower)
        : Component(id, name, power, temp), portCount(ports),
          usbStandard(standard), powerPerPortWatts(portPower) {}

    void powerOn() override {
        isWorking = true;
        std::cout << "USB-контроллер " << name << " (" << usbStandard << ", "
                  << portCount << " портов) активен.\n";
    }

    void powerOff() override {
        isWorking = false;
        connectedDevices.clear();
        std::cout << "USB-контроллер " << name << " обесточен.\n";
    }

    void connectDevice(Component* device) {
        if (device == nullptr) {
            std::cout << "Нельзя подключить пустое устройство.\n";
            return;
        }
        if (static_cast<int>(connectedDevices.size()) >= portCount) {
            std::cout << "Все порты USB-контроллера " << name << " заняты.\n";
            return;
        }
        connectedDevices.push_back(device);
        device->powerOn();
        std::cout << "Устройство подключено к USB-контроллеру " << name
                  << ". Занято портов: " << connectedDevices.size() << "/"
                  << portCount << ".\n";
    }

    void disconnectDevice(Component* device) {
        for (auto it = connectedDevices.begin(); it != connectedDevices.end(); ++it) {
            if (*it == device) {
                (*it)->powerOff();
                connectedDevices.erase(it);
                std::cout << "Устройство отключено от USB-контроллера " << name << ".\n";
                return;
            }
        }
        std::cout << "Устройство не найдено на USB-контроллере " << name << ".\n";
    }

    bool checkPowerBudget(double requestedWatts) const {
        double available = powerPerPortWatts * portCount;
        bool enough = requestedWatts <= available;
        std::cout << "Бюджет питания USB " << name << ": запрошено "
                  << requestedWatts << " Вт из " << available << " Вт — "
                  << (enough ? "достаточно." : "недостаточно.") << "\n";
        return enough;
    }
};

} // namespace hardware

#endif
