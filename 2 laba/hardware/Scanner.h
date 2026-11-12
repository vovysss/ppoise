#ifndef SCANNER_H
#define SCANNER_H

#include "Component.h"
#include "../software/File.h"
#include <string>
#include <iostream>

namespace hardware {

class Scanner : public Component {
private:
    int resolutionDpi;
    int colorDepthBits;
    int scannedPages;
    bool lampWarmedUp;

public:
    Scanner(std::string id, std::string name, double power, double temp,
            int dpiValue, int depth)
        : Component(id, name, power, temp), resolutionDpi(dpiValue),
          colorDepthBits(depth), scannedPages(0), lampWarmedUp(false) {}

    void powerOn() override {
        isWorking = true;
        std::cout << "Сканер " << name << " инициализирован.\n";
    }

    void powerOff() override {
        isWorking = false;
        lampWarmedUp = false;
        std::cout << "Сканер " << name << " выключен.\n";
    }

    void scanPage(const software::File& source) {
        if (!isWorking) {
            std::cout << "Сканер " << name << " выключен.\n";
            return;
        }
        if (!lampWarmedUp) {
            calibrateLamp();
        }
        ++scannedPages;
        std::cout << "Сканер " << name << " оцифровал документ " << source.getName()
                  << " в " << resolutionDpi << " DPI (" << colorDepthBits
                  << " бит/канал).\n";
    }

    void calibrateLamp() {
        lampWarmedUp = true;
        std::cout << "Лампа сканера " << name << " прогрета и откалибрована.\n";
    }
};

} // namespace hardware

#endif
