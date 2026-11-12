#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "Component.h"
#include <string>
#include <iostream>

namespace hardware {

class Keyboard : public Component {
private:
    std::string layout;
    int keyCount;
    bool isBacklit;
    int backlightLevel;
    char lastPressedKey;

public:
    Keyboard(std::string id, std::string name, double power, double temp,
             std::string keyboardLayout, int keys)
        : Component(id, name, power, temp), layout(keyboardLayout),
          keyCount(keys), isBacklit(false), backlightLevel(0),
          lastPressedKey('\0') {}

    void powerOn() override {
        isWorking = true;
        std::cout << "Клавиатура " << name << " (" << layout << ", "
                  << keyCount << " клавиш) подключена.\n";
    }

    void powerOff() override {
        isWorking = false;
        std::cout << "Клавиатура " << name << " отключена.\n";
    }

    void pressKey(char key) {
        if (!isWorking) {
            std::cout << "Клавиатура " << name << " не активна.\n";
            return;
        }
        lastPressedKey = key;
        std::cout << "На клавиатуре " << name << " нажата клавиша '" << key << "'.\n";
    }

    void toggleBacklight() {
        isBacklit = !isBacklit;
        backlightLevel = isBacklit ? 50 : 0;
        std::cout << "Подсветка клавиатуры " << name << " "
                  << (isBacklit ? "включена (50%)." : "выключена.") << "\n";
    }

    void typeText(const std::string& text) {
        std::cout << "Клавиатура " << name << " вводит текст: " << text << "\n";
        if (!text.empty()) lastPressedKey = text.back();
    }
};

} // namespace hardware

#endif
