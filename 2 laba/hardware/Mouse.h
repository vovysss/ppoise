#ifndef MOUSE_H
#define MOUSE_H

#include "Component.h"
#include <string>
#include <iostream>

namespace hardware {

class Mouse : public Component {
private:
    int dpi;
    int buttonCount;
    bool isWireless;
    int batteryPercent;
    int cursorX;
    int cursorY;

public:
    Mouse(std::string id, std::string name, double power, double temp,
          int sensorDpi, int buttons, bool wireless)
        : Component(id, name, power, temp), dpi(sensorDpi),
          buttonCount(buttons), isWireless(wireless),
          batteryPercent(wireless ? 100 : 0), cursorX(0), cursorY(0) {}

    void powerOn() override {
        isWorking = true;
        std::cout << "Мышь " << name << " (" << dpi << " DPI, "
                  << (isWireless ? "беспроводная" : "проводная")
                  << ", заряд " << batteryPercent << "%) готова к работе.\n";
    }

    void powerOff() override {
        isWorking = false;
        std::cout << "Мышь " << name << " отключена.\n";
    }

    void moveCursor(int dx, int dy) {
        cursorX += dx;
        cursorY += dy;
        std::cout << "Курсор перемещен в точку (" << cursorX << ", " << cursorY << ").\n";
    }

    void clickButton(int buttonIndex) {
        if (buttonIndex < 1 || buttonIndex > buttonCount) {
            std::cout << "Кнопка #" << buttonIndex << " отсутствует на мыши " << name << ".\n";
            return;
        }
        std::cout << "Мышь " << name << ": нажата кнопка #" << buttonIndex << ".\n";
    }

    void changeDpi(int newDpi) {
        dpi = newDpi;
        std::cout << "Чувствительность мыши " << name << " изменена на " << dpi << " DPI.\n";
    }
};

} // namespace hardware

#endif
