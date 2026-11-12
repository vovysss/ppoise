#ifndef WIFI_ADAPTER_H
#define WIFI_ADAPTER_H

#include "Component.h"
#include "../network/NetworkPacket.h"
#include <string>
#include <iostream>

namespace hardware {

class WiFiAdapter : public Component {
private:
    std::string ssid;
    int signalStrengthDbm;
    std::string band;
    bool isConnected;

public:
    WiFiAdapter(std::string id, std::string name, double power, double temp,
                std::string preferredBand)
        : Component(id, name, power, temp), ssid(""),
          signalStrengthDbm(-100), band(preferredBand), isConnected(false) {}

    void powerOn() override {
        isWorking = true;
        std::cout << "Wi-Fi адаптер " << name << " (" << band << ") включен.\n";
    }

    void powerOff() override {
        isConnected = false;
        isWorking = false;
        std::cout << "Wi-Fi адаптер " << name << " выключен.\n";
    }

    void connectToNetwork(const std::string& networkName) {
        ssid = networkName;
        isConnected = true;
        signalStrengthDbm = -45;
        std::cout << "Wi-Fi адаптер " << name << " подключен к сети " << ssid
                  << " (" << signalStrengthDbm << " дБм).\n";
    }

    void scanNetworks() {
        std::cout << "Wi-Fi адаптер " << name << " обнаружил сети: HomeNet, Office-5G, Guest.\n";
    }

    void sendPacket(network::NetworkPacket& packet) {
        if (!isConnected) {
            std::cout << "Wi-Fi адаптер " << name << " не подключен к сети.\n";
            return;
        }
        packet.transmit();
    }
};

} // namespace hardware

#endif
