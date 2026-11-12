#ifndef TOUCHPAD_H
#define TOUCHPAD_H

#include "Component.h"
#include <string>
#include <iostream>

namespace hardware {

class Touchpad : public Component {
private:
    int widthMM;
    int heightMM;
    bool gestureEnabled;
    std::string lastGesture;

public:
    Touchpad(std::string id, std::string name, double power, double temp,
             int width, int height)
        : Component(id, name, power, temp), widthMM(width), heightMM(height),
          gestureEnabled(true), lastGesture("none") {}

    void powerOn() override {
        isWorking = true;
        std::cout << "Тачпад " << name << " (" << widthMM << "x" << heightMM
                  << " мм) активирован.\n";
    }

    void powerOff() override {
        isWorking = false;
        std::cout << "Тачпад " << name << " отключен.\n";
    }

    void detectGesture(const std::string& gesture) {
        if (!gestureEnabled) {
            std::cout << "Жесты на тачпаде " << name << " отключены.\n";
            return;
        }
        lastGesture = gesture;
        std::cout << "Тачпад " << name << " распознал жест: " << lastGesture << ".\n";
    }

    void scroll(int delta) {
        std::cout << "Тачпад " << name << " прокручивает содержимое на "
                  << delta << " единиц.\n";
    }

    void tap() {
        std::cout << "Тачпад " << name << ": выполнен клик касанием.\n";
    }
};

} // namespace hardware

#endif
