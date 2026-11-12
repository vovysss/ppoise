#ifndef COMPUTER_CASE_H
#define COMPUTER_CASE_H

#include "Component.h"
#include <string>
#include <iostream>

namespace hardware {

class ComputerCase : public Component {
private:
    std::string formFactor; // Full-Tower, Mid-Tower, Mini-ITX
    double maxGPU_LengthMM;
    bool isSidePanelOpen;

public:
    ComputerCase(std::string id, std::string name, double power, double temp, 
                 std::string form, double maxGpuLen)
        : Component(id, name, power, temp), formFactor(form), 
          maxGPU_LengthMM(maxGpuLen), isSidePanelOpen(false) {}

    void powerOn() override {
        isWorking = true;
        std::cout << "Корпус " << name << " (" << formFactor << ", макс. длина GPU "
                  << maxGPU_LengthMM << " мм) готов к эксплуатации.\n";
    }

    void powerOff() override {
        isWorking = false;
        std::cout << "Системный корпус " << name << " переведен в выключенное состояние.\n";
    }

    void toggleSidePanel() {
        isSidePanelOpen = !isSidePanelOpen;
        std::cout << "Боковая панель корпуса " << (isSidePanelOpen ? "открыта." : "закрыта.") << "\n";
    }

    std::string getFormFactor() const { return formFactor; }
};

} // namespace hardware

#endif