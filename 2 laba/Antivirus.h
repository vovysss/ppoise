#ifndef ANTIVIRUS_H
#define ANTIVIRUS_H

#include "File.h"
#include <string>
#include <iostream>

namespace software {

class Antivirus {
private:
    std::string signatureVersion;
    int threatsFound;
    bool isActive;

public:
    Antivirus(std::string version)
        : signatureVersion(version), threatsFound(0), isActive(false) {}

    void activate() {
        isActive = true;
        std::cout << "Антивирус активирован (базы " << signatureVersion << ").\n";
    }

    void deactivate() {
        isActive = false;
        std::cout << "Антивирус отключен.\n";
    }

    bool scanFile(const File& file) {
        if (!isActive) {
            std::cout << "Антивирус не активен.\n";
            return false;
        }
        bool clean = file.getSize() < 500;
        if (!clean) {
            ++threatsFound;
            std::cout << "Угроза обнаружена в файле " << file.getName() << ".\n";
        } else {
            std::cout << "Файл " << file.getName() << " безопасен.\n";
        }
        return clean;
    }

    void quarantine(const std::string& fileName) {
        std::cout << "Файл " << fileName << " помещен в карантин.\n";
    }

    void updateSignatures(const std::string& newVersion) {
        signatureVersion = newVersion;
        std::cout << "Антивирусные базы обновлены до " << signatureVersion << ".\n";
    }
};

} // namespace software

#endif
