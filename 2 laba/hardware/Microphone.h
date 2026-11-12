#ifndef MICROPHONE_H
#define MICROPHONE_H

#include "Component.h"
#include <string>
#include <iostream>

namespace hardware {

class Microphone : public Component {
private:
    double sensitivityDb;
    bool isMuted;
    int sampleRateHz;
    int recordedSeconds;

public:
    Microphone(std::string id, std::string name, double power, double temp,
               double sensitivity, int sampleRate)
        : Component(id, name, power, temp), sensitivityDb(sensitivity),
          isMuted(false), sampleRateHz(sampleRate), recordedSeconds(0) {}

    void powerOn() override {
        isWorking = true;
        isMuted = false;
        std::cout << "Микрофон " << name << " активирован ("
                  << sensitivityDb << " дБ, " << sampleRateHz << " Гц).\n";
    }

    void powerOff() override {
        isWorking = false;
        std::cout << "Микрофон " << name << " выключен.\n";
    }

    void captureAudio(int seconds) {
        if (isMuted) {
            std::cout << "Микрофон " << name << " заглушен, запись невозможна.\n";
            return;
        }
        recordedSeconds += seconds;
        std::cout << "Микрофон " << name << " записал " << seconds
                  << " секунд аудио.\n";
    }

    void mute() {
        isMuted = true;
        std::cout << "Микрофон " << name << " заглушен.\n";
    }

    void unmute() {
        isMuted = false;
        std::cout << "Микрофон " << name << " снова активен.\n";
    }
};

} // namespace hardware

#endif
