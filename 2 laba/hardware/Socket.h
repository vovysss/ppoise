#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <iostream>

namespace hardware {

class Socket {
private:
    std::string socketName;
    int pinCount;
    bool isOccupied;

public:
    Socket(std::string name, int pins)
        : socketName(name), pinCount(pins), isOccupied(false) {}

    void lockSocket() {
        isOccupied = true;
        std::cout << "Сокет " << socketName << " (" << pinCount << " контактов) зафиксирован.\n";
    }

    void unlockSocket() {
        isOccupied = false;
        std::cout << "Сокет " << socketName << " разблокирован.\n";
    }

    std::string getName() const { return socketName; }
    int getPinCount() const { return pinCount; }
    bool occupied() const { return isOccupied; }
};

} // namespace hardware

#endif