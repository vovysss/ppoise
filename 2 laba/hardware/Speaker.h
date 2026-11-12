#ifndef SPEAKER_H
#define SPEAKER_H

#include "Component.h"
#include <string>
#include <iostream>

namespace hardware {

class Speaker : public Component {
private:
    int channelCount;
    int volumePercent;
    bool isMuted;
    double frequencyRangeHz;

public:
    Speaker(std::string id, std::string name, double power, double temp,
            int channels, double frequencyRange)
        : Component(id, name, power, temp), channelCount(channels),
          volumePercent(50), isMuted(false), frequencyRangeHz(frequencyRange) {}

    void powerOn() override {
        isWorking = true;
        std::cout << "Акустическая система " << name << " (" << channelCount
                  << " канала, диапазон до " << frequencyRangeHz
                  << " Гц) включена.\n";
    }

    void powerOff() override {
        isWorking = false;
        std::cout << "Акустическая система " << name << " выключена.\n";
    }

    void playSound(const std::string& soundName) {
        if (isMuted) {
            std::cout << "Звук отключен, воспроизведение " << soundName << " пропущено.\n";
            return;
        }
        std::cout << "Акустическая система " << name << " воспроизводит: "
                  << soundName << " (" << volumePercent << "%).\n";
    }

    void increaseVolume(int delta) {
        volumePercent += delta;
        if (volumePercent > 100) volumePercent = 100;
        std::cout << "Громкость " << name << " повышена до " << volumePercent << "%.\n";
    }

    void decreaseVolume(int delta) {
        volumePercent -= delta;
        if (volumePercent < 0) volumePercent = 0;
        std::cout << "Громкость " << name << " понижена до " << volumePercent << "%.\n";
    }

    void mute() {
        isMuted = !isMuted;
        std::cout << "Акустическая система " << name << " "
                  << (isMuted ? "заглушена." : "включена.") << "\n";
    }
};

} // namespace hardware

#endif
