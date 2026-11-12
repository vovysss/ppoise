#ifndef MOTHERBOARD_H
#define MOTHERBOARD_H

#include "Component.h"
#include "Processor.h"
#include "RAMModule.h" // <--- 1. Подключаем модуль памяти здесь
#include <vector>
#include <string>
#include <iostream>

namespace hardware {

class Motherboard : public Component {
private:
    std::string chipset;
    int socketType;
    
    Processor* installedProcessor;
    
    int maxRamSlots;
    int usedRamSlots;
    
    std::vector<RAMModule*> ramSlots; // <--- 2. Добавляем вектор слотов в приватную секцию

public:
    Motherboard(std::string id, std::string name, double power, double temp, 
                std::string chipsetModel, int socket, int ramSlotsCount)
        : Component(id, name, power, temp), chipset(chipsetModel), 
          socketType(socket), installedProcessor(nullptr), 
          maxRamSlots(ramSlotsCount), usedRamSlots(0) {}

    void powerOn() override {
        isWorking = true;
        std::cout << "Материнская плата " << name << " (Чипсет: " << chipset << ") инициализирована.\n";
        if (installedProcessor) {
            installedProcessor->powerOn();
        }
        for (auto* ram : ramSlots) {
            if (ram) ram->powerOn();
        }
    }

    void powerOff() override {
        for (auto* ram : ramSlots) {
            if (ram) ram->powerOff();
        }
        if (installedProcessor) {
            installedProcessor->powerOff();
        }
        isWorking = false;
        std::cout << "Материнская плата " << name << " обесточена.\n";
    }

    void installProcessor(Processor* cpu, int cpuSocketType) {
        if (installedProcessor != nullptr) {
            std::cout << "Ошибка: на материнской плате уже установлен процессор!\n";
            return;
        }

        if (cpuSocketType != this->socketType) {
            std::cout << "Ошибка совместимости: сокет процессора (" << cpuSocketType 
                      << ") не подходит к сокету платы (" << socketType << ")!\n";
            return;
        }

        installedProcessor = cpu;
        std::cout << "Процессор успешно установлен в сокет материнской платы.\n";
    }

    // <--- 3. Вот сюда добавляем метод установки оперативной памяти:
    void installRAM(RAMModule* ram) {
        if (usedRamSlots < maxRamSlots) {
            ramSlots.push_back(ram);
            ++usedRamSlots;
            std::cout << "Модуль оперативной памяти успешно установлен в слот материнской платы ("
                      << usedRamSlots << "/" << maxRamSlots << ").\n";
        } else {
            std::cout << "Ошибка: все слоты оперативной памяти заняты!\n";
        }
    }

    bool checkSystemHealth() {
        if (!checkHealth()) return false;
        if (installedProcessor && !installedProcessor->checkHealth()) {
            return false;
        }
        return true;
    }
};

} // namespace hardware

#endif