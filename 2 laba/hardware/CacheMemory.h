#ifndef CACHE_MEMORY_H
#define CACHE_MEMORY_H

#include <string>
#include <iostream>

namespace hardware {

class CacheMemory {
private:
    std::string cacheLevel; // L1, L2, L3
    size_t sizeKB;
    double latencyNs;      // Задержка в наносекундах
    bool isActive;

public:
    CacheMemory(std::string level, size_t size, double latency)
        : cacheLevel(level), sizeKB(size), latencyNs(latency), isActive(false) {}

    void activate() {
        isActive = true;
        std::cout << "Кэш-память " << cacheLevel << " (" << sizeKB << " КБ, задержка: " << latencyNs << " нс) активна.\n";
    }

    void deactivate() {
        isActive = false;
        std::cout << "Кэш-память " << cacheLevel << " отключена.\n";
    }

    void fetchCache(size_t address) {
        if (isActive) {
            std::cout << "Чтение из кэша " << cacheLevel << " по адресу 0x" << std::hex << address << std::dec << " (задержка " << latencyNs << " нс).\n";
        }
    }

    std::string getLevel() const { return cacheLevel; }
    size_t getSize() const { return sizeKB; }
};

} // namespace hardware

#endif