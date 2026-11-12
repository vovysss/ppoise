#ifndef GRAPHICS_CARD_H
#define GRAPHICS_CARD_H

#include "Component.h"
#include "GPUCore.h"
#include "VRAM.h"
#include <vector>
#include <string>
#include <iostream>

namespace hardware {

class GraphicsCard : public Component {
private:
    std::string chipsetModel;
    std::vector<GPUCore> gpuCores; // Композиция: видеокарта владеет ядрами
    VRAM* videoMemory;             // Агрегация: ссылка на VRAM
    double coreClockMHz;

public:
    GraphicsCard(std::string id, std::string name, double power, double temp, 
                 std::string chipset, double clock, int coreCount, VRAM* vramPtr)
        : Component(id, name, power, temp), chipsetModel(chipset), 
          videoMemory(vramPtr), coreClockMHz(clock) {
        // Создаем ядра GPU внутри конструктора (композиция)
        for (int i = 1; i <= coreCount; ++i) {
            gpuCores.emplace_back(i, coreClockMHz);
        }
    }

    void powerOn() override {
        isWorking = true;
        std::cout << "Видеокарта " << name << " (Чипсет: " << chipsetModel << ") включена.\n";
        for (auto& core : gpuCores) {
            core.activate();
        }
        if (videoMemory) {
            videoMemory->powerOn();
        }
    }

    void powerOff() override {
        if (videoMemory) {
            videoMemory->powerOff();
        }
        for (auto& core : gpuCores) {
            core.deactivate();
        }
        isWorking = false;
        std::cout << "Видеокарта " << name << " обесточена.\n";
    }

    // Содержательный метод рендеринга
    void renderFrame(double complexity) {
        std::cout << "Запуск рендеринга сцены на видеокарте " << name << " (сложность: " << complexity << ")...\n";
        for (auto& core : gpuCores) {
            core.setLoad(complexity);
        }
    }
};

} // namespace hardware

#endif