#ifndef UPDATE_SERVICE_H
#define UPDATE_SERVICE_H

#include <string>
#include <iostream>

namespace software {

class UpdateService {
private:
    std::string currentVersion;
    std::string availableVersion;
    bool isUpdateAvailable;

public:
    UpdateService(std::string version)
        : currentVersion(version), availableVersion(version),
          isUpdateAvailable(false) {}

    void checkForUpdates(const std::string& latestVersion) {
        availableVersion = latestVersion;
        isUpdateAvailable = (latestVersion != currentVersion);
        std::cout << "Проверка обновлений: текущая " << currentVersion
                  << ", доступна " << availableVersion << ". "
                  << (isUpdateAvailable ? "Требуется обновление." : "Система актуальна.")
                  << "\n";
    }

    void installUpdate() {
        if (!isUpdateAvailable) {
            std::cout << "Нет доступных обновлений.\n";
            return;
        }
        currentVersion = availableVersion;
        isUpdateAvailable = false;
        std::cout << "Обновление установлено. Новая версия: " << currentVersion << ".\n";
    }

    void rollback() {
        std::cout << "Выполнен откат системы к предыдущей версии.\n";
    }

    void notifyUser() const {
        std::cout << "Уведомление: доступна версия " << availableVersion << ".\n";
    }
};

} // namespace software

#endif
