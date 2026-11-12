#ifndef PRINTER_H
#define PRINTER_H

#include "Component.h"
#include "../software/File.h"
#include <string>
#include <iostream>

namespace hardware {

class Printer : public Component {
private:
    int inkLevelPercent;
    int paperCount;
    bool duplexEnabled;
    int printedPageCount;

public:
    Printer(std::string id, std::string name, double power, double temp,
            int inkLevel, int paper)
        : Component(id, name, power, temp), inkLevelPercent(inkLevel),
          paperCount(paper), duplexEnabled(false), printedPageCount(0) {}

    void powerOn() override {
        isWorking = true;
        std::cout << "Принтер " << name << " прогрет и готов к печати.\n";
    }

    void powerOff() override {
        isWorking = false;
        std::cout << "Принтер " << name << " выключен.\n";
    }

    void printDocument(const software::File& document) {
        if (inkLevelPercent <= 0 || paperCount <= 0) {
            std::cout << "Печать невозможна: проверьте чернила и бумагу.\n";
            return;
        }
        int pages = 1 + static_cast<int>(document.getSize() / 2);
        paperCount -= pages;
        inkLevelPercent -= pages;
        if (paperCount < 0) paperCount = 0;
        if (inkLevelPercent < 0) inkLevelPercent = 0;
        printedPageCount += pages;
        std::cout << "Принтер " << name << " напечатал документ "
                  << document.getName() << " (" << pages << " стр., дуплекс: "
                  << (duplexEnabled ? "вкл" : "выкл") << ").\n";
    }

    void refillInk(int percent) {
        inkLevelPercent += percent;
        if (inkLevelPercent > 100) inkLevelPercent = 100;
        std::cout << "Чернила принтера " << name << " заправлены до "
                  << inkLevelPercent << "%.\n";
    }

    void loadPaper(int sheets) {
        paperCount += sheets;
        std::cout << "В принтер " << name << " загружено " << sheets
                  << " листов, всего: " << paperCount << ".\n";
    }
};

} // namespace hardware

#endif
