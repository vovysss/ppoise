#ifndef ROUTER_H
#define ROUTER_H

#include "FirewallRule.h"
#include "NetworkPacket.h"
#include <vector>
#include <string>
#include <iostream>

namespace network {

class Router {
private:
    std::string ipAddress;
    bool dhcpEnabled;
    int connectedClientCount;
    std::vector<FirewallRule> firewallRules;
    std::vector<NetworkPacket> packetBuffer;

public:
    Router(std::string address, bool dhcp)
        : ipAddress(address), dhcpEnabled(dhcp), connectedClientCount(0) {}

    void start() {
        std::cout << "Маршрутизатор " << ipAddress << " запущен (DHCP: "
                  << (dhcpEnabled ? "вкл" : "выкл") << ").\n";
    }

    void stop() {
        connectedClientCount = 0;
        packetBuffer.clear();
        std::cout << "Маршрутизатор " << ipAddress << " остановлен.\n";
    }

    void assignIP(const std::string& clientName) {
        if (!dhcpEnabled) {
            std::cout << "DHCP отключен, клиенту " << clientName
                      << " нужен статический адрес.\n";
            return;
        }
        ++connectedClientCount;
        std::cout << "Клиент " << clientName << " получил IP от маршрутизатора "
                  << ipAddress << ". Всего клиентов: " << connectedClientCount << ".\n";
    }

    void forwardPacket(NetworkPacket& packet) {
        for (const auto& rule : firewallRules) {
            if (rule.permits(packet)) {
                packetBuffer.push_back(packet);
                packet.transmit();
                std::cout << "Маршрутизатор " << ipAddress
                          << " переслал пакет по правилу firewall.\n";
                return;
            }
        }
        std::cout << "Маршрутизатор " << ipAddress
                  << " отклонил пакет: нет разрешающего правила.\n";
    }

    void addFirewallRule(const FirewallRule& rule) {
        firewallRules.push_back(rule);
        rule.apply();
    }

    void restart() {
        packetBuffer.clear();
        std::cout << "Маршрутизатор " << ipAddress << " перезагружен.\n";
    }
};

} // namespace network

#endif
