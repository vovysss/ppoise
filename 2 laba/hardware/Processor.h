#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "../exceptions/OverheatingException.h"
#include "Component.h"
#include "Core.h"
#include <vector>
#include <string>

namespace hardware {

class Processor : public Component {
private:
    double baseFrequency;
    int socketType;
    std::vector<Core> cores; // Ассоциация: Композиция (ядра принадлежат процессору)

public:
    Processor(std::string id, std::string name, double power, double temp, 
              double baseFreq, int socket, int physicalCores)
        : Component(id, name, power, temp), baseFrequency(baseFreq), socketType(socket) {
        
        // Создаем ядра при инициализации процессора
        for (int i = 0; i < physicalCores; ++i) {
            cores.emplace_back(i + 1, baseFrequency);
        }
    }

    void powerOn() override {
        isWorking = true;
        std::cout << "Процессор " << name << " подан под напряжение. Сокет: "
                  << socketType << ". Ядер активировано: " << cores.size() << ".\n";
    }

    void powerOff() override {
        isWorking = false;
        std::cout << "Процессор " << name << " выключен.\n";
    }

    // Содержательный метод (бизнес-логика)
  void overclock(double percent) {
        baseFrequency *= (1.0 + percent / 100.0);
        temperature += 25.0; // Сильный рост температуры при разгоне
        
        std::cout << "Попытка разгона процессора на " << percent << "%...\n";

        // Проверяем условие и выбрасываем наше кастомное исключение
        if (temperature > 85.0) {
            throw exceptions::OverheatingException(name, temperature);
        }

        std::cout << "Разгон успешен! Новая частота: " << baseFrequency << " ГГц, температура: " << temperature << "°C.\n";
    }

    void distributeLoad(double totalLoad) {
        std::cout << "Распределение общей нагрузки " << totalLoad << "% по ядрам:\n";
        for (auto& core : cores) {
            core.setLoad(totalLoad / cores.size());
            core.executeInstruction();
        }
    }
};

} // namespace hardware

#endif