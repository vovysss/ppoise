#ifndef DRIVER_H
#define DRIVER_H

#include <string>
#include <iostream>

namespace software {

class Driver {
private:
    std::string driverName;
    std::string deviceTarget;
    std::string version;
    bool isLoaded;

public:
    Driver(std::string name, std::string target, std::string ver)
        : driverName(name), deviceTarget(target), version(ver), isLoaded(false) {}

    void loadDriver() {
        isLoaded = true;
        std::cout << "Драйвер [" << driverName << "] для устройства " << deviceTarget << " (версия " << version << ") загружен в память.\n";
    }

    void unloadDriver() {
        isLoaded = false;
        std::cout << "Драйвер [" << driverName << "] выгружен.\n";
    }

    std::string getDriverName() const { return driverName; }
    bool getStatus() const { return isLoaded; }
};

} // namespace software

#endif