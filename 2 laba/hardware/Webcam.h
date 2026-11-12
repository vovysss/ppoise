#ifndef WEBCAM_H
#define WEBCAM_H

#include "Component.h"
#include <string>
#include <iostream>

namespace hardware {

class Webcam : public Component {
private:
    double resolutionMP;
    int fps;
    bool isStreaming;
    int capturedPhotos;

public:
    Webcam(std::string id, std::string name, double power, double temp,
           double megapixels, int frameRate)
        : Component(id, name, power, temp), resolutionMP(megapixels),
          fps(frameRate), isStreaming(false), capturedPhotos(0) {}

    void powerOn() override {
        isWorking = true;
        std::cout << "Веб-камера " << name << " (" << resolutionMP
                  << " Мп, " << fps << " FPS) готова.\n";
    }

    void powerOff() override {
        isStreaming = false;
        isWorking = false;
        std::cout << "Веб-камера " << name << " выключена.\n";
    }

    void startStream() {
        if (!isWorking) {
            std::cout << "Сначала включите веб-камеру " << name << ".\n";
            return;
        }
        isStreaming = true;
        std::cout << "Веб-камера " << name << " начала трансляцию в "
                  << fps << " FPS.\n";
    }

    void stopStream() {
        isStreaming = false;
        std::cout << "Трансляция с веб-камеры " << name << " остановлена.\n";
    }

    void capturePhoto() {
        ++capturedPhotos;
        std::cout << "Веб-камера " << name << " сделала снимок #"
                  << capturedPhotos << ".\n";
    }
};

} // namespace hardware

#endif
