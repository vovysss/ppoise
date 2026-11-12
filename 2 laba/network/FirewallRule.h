#ifndef FIREWALL_RULE_H
#define FIREWALL_RULE_H

#include "NetworkPacket.h"
#include <string>
#include <iostream>

namespace network {

class FirewallRule {
private:
    std::string ruleName;
    std::string sourceIP;
    std::string destinationIP;
    int port;
    bool isAllowed;

public:
    FirewallRule(std::string name, std::string source, std::string destination,
                 int targetPort, bool allowed)
        : ruleName(name), sourceIP(source), destinationIP(destination),
          port(targetPort), isAllowed(allowed) {}

    bool permits(const NetworkPacket& packet) const {
        return isAllowed && packet.getDestination() == destinationIP;
    }

    void apply() const {
        std::cout << "Правило firewall [" << ruleName << "] применено: "
                  << sourceIP << " -> " << destinationIP << ":" << port
                  << " (" << (isAllowed ? "разрешено" : "запрещено") << ").\n";
    }

    void revoke() const {
        std::cout << "Правило firewall [" << ruleName << "] отозвано.\n";
    }

    void describe() const {
        std::cout << "FirewallRule " << ruleName << ": " << sourceIP << " -> "
                  << destinationIP << ":" << port << ", "
                  << (isAllowed ? "ALLOW" : "DENY") << ".\n";
    }
};

} // namespace network

#endif
