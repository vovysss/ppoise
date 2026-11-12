#ifndef NETWORK_CARD_H
#define NETWORK_CARD_H

#include "../hardware/Component.h"
#include "NetworkPacket.h"
#include <string>
#include <iostream>

namespace network {

class NetworkCard : public hardware::Component {
private:
    std::string macAddress;
    double maxSpeedMbps;
    bool isConnected;

public:
    NetworkCard(std::string id, std::string name, double power, double temp, 
                std::string mac, double speed)
        : hardware::Component(id, name, power, temp), macAddress(mac), 
          maxSpeedMbps(speed), isConnected(false) {}

    void powerOn() override {
        isWorking = true;
        isConnected = true;
        std::cout << "Сетевая карта " << name << " (MAC: " << macAddress << ") подключена к сети. Скорость: " << maxSpeedMbps << " Мбит/с.\n";
    }

    void powerOff() override {
        isConnected = false;
        isWorking = false;
        std::cout << "Сетевая карта " << name << " отключена от сети.\n";
    }

    void sendPacket(NetworkPacket& packet) {
        if (isConnected) {
            std::cout << "Сетевая карта " << name << " передает пакет...\n";
            packet.transmit();
        } else {
            std::cout << "Ошибка: нет соединения с сетью для отправки пакета!\n";
        }
    }

    std::string getMacAddress() const { return macAddress; }
    bool getStatus() const { return isConnected; }
};

} // namespace network

#endif