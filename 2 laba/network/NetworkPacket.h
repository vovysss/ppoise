#ifndef NETWORK_PACKET_H
#define NETWORK_PACKET_H

#include <string>
#include <iostream>

namespace network {

class NetworkPacket {
private:
    int packetId;
    std::string destinationIP;
    size_t payloadSizeKB;
    bool isDelivered;

public:
    NetworkPacket(int id, std::string destIP, size_t size)
        : packetId(id), destinationIP(destIP), payloadSizeKB(size), isDelivered(false) {}

    void transmit() {
        isDelivered = true;
        std::cout << "Сетевой пакет #" << packetId << " успешно отправлен на IP: " << destinationIP << " (" << payloadSizeKB << " КБ).\n";
    }

    int getId() const { return packetId; }
    std::string getDestination() const { return destinationIP; }
    bool getStatus() const { return isDelivered; }
};

} // namespace network

#endif