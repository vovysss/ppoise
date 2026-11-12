#ifndef SOUND_CARD_H
#define SOUND_CARD_H

#include "Component.h"
#include <string>
#include <iostream>

namespace hardware {

class SoundCard : public Component {
private:
    int channels;
    int sampleRateKHz;
    int bitDepth;
    std::string activeOutput;

public:
    SoundCard(std::string id, std::string name, double power, double temp,
              int channelCount, int sampleRate, int depth)
        : Component(id, name, power, temp), channels(channelCount),
          sampleRateKHz(sampleRate), bitDepth(depth),
          activeOutput("Speakers") {}

    void powerOn() override {
        isWorking = true;
        std::cout << "Звуковая карта " << name << " (" << channels
                  << " каналов, " << bitDepth << " бит/" << sampleRateKHz
                  << " кГц) запущена.\n";
    }

    void powerOff() override {
        isWorking = false;
        std::cout << "Звуковая карта " << name << " выключена.\n";
    }

    void synthesizeSound(int toneHz) {
        std::cout << "Звуковая карта " << name << " синтезирует тон "
                  << toneHz << " Гц в " << channels << " каналах.\n";
    }

    void routeAudio(const std::string& outputName) {
        activeOutput = outputName;
        std::cout << "Аудиопоток звуковой карты " << name << " направлен на "
                  << activeOutput << ".\n";
    }
};

} // namespace hardware

#endif
