#ifndef MONITOR_H
#define MONITOR_H

#include "Component.h"
#include "GraphicsCard.h"
#include <string>
#include <iostream>

namespace hardware {

class Monitor : public Component {
private:
    double screenSizeInch;
    int resolutionWidth;
    int resolutionHeight;
    int refreshRateHz;
    std::string panelType;
    int brightnessNits;
    std::string currentInput;
    GraphicsCard* connectedGPU;

public:
    Monitor(std::string id, std::string name, double power, double temp,
            double size, int width, int height, int refresh,
            std::string panel, int brightness, GraphicsCard* gpu = nullptr)
        : Component(id, name, power, temp), screenSizeInch(size),
          resolutionWidth(width), resolutionHeight(height),
          refreshRateHz(refresh), panelType(panel),
          brightnessNits(brightness), currentInput("DisplayPort"),
          connectedGPU(gpu) {}

    void powerOn() override {
        isWorking = true;
        std::cout << "Монитор " << name << " (" << screenSizeInch << "\", "
                  << resolutionWidth << "x" << resolutionHeight << ", "
                  << refreshRateHz << " Гц, " << panelType << ") включен.\n";
    }

    void powerOff() override {
        isWorking = false;
        std::cout << "Монитор " << name << " переведен в режим ожидания.\n";
    }

    void adjustBrightness(int delta) {
        brightnessNits += delta;
        if (brightnessNits < 0) brightnessNits = 0;
        if (brightnessNits > 1000) brightnessNits = 1000;
        std::cout << "Яркость монитора " << name << " установлена на "
                  << brightnessNits << " нит.\n";
    }

    void switchInput(const std::string& input) {
        currentInput = input;
        std::cout << "Монитор " << name << " переключен на вход " << currentInput << ".\n";
    }

    void calibrateColors() {
        std::cout << "Калибровка цветопередачи монитора " << name
                  << " завершена (sRGB, " << brightnessNits << " нит).\n";
    }

    void displayFrame(const std::string& scene) {
        if (!isWorking) {
            std::cout << "Монитор " << name << " выключен, кадр не отображен.\n";
            return;
        }
        if (connectedGPU) {
            connectedGPU->renderFrame(65.0);
        }
        std::cout << "Монитор " << name << " отображает сцену: " << scene << ".\n";
    }
};

} // namespace hardware

#endif
