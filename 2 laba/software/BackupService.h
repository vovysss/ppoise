#ifndef BACKUP_SERVICE_H
#define BACKUP_SERVICE_H

#include "FileSystem.h"
#include <string>
#include <iostream>

namespace software {

class BackupService {
private:
    std::string backupPath;
    std::string lastBackupTime;
    int backupCount;
    FileSystem* source;

public:
    BackupService(std::string path, FileSystem* sourceFs)
        : backupPath(path), lastBackupTime("никогда"), backupCount(0),
          source(sourceFs) {}

    void createBackup() {
        if (source == nullptr) {
            std::cout << "Источник резервного копирования не задан.\n";
            return;
        }
        ++backupCount;
        lastBackupTime = "2026-11-12 12:00";
        std::cout << "Создана резервная копия #" << backupCount << " в "
                  << backupPath << ".\n";
    }

    void restoreBackup(const std::string& backupName) {
        std::cout << "Восстановление из копии " << backupName << " в "
                  << backupPath << " завершено.\n";
    }

    bool verifyBackup() const {
        bool ok = source != nullptr && backupCount > 0;
        std::cout << "Проверка резервной копии: "
                  << (ok ? "успешно." : "копия отсутствует.") << "\n";
        return ok;
    }

    void scheduleBackup(int intervalHours) {
        std::cout << "Резервное копирование запланировано каждые "
                  << intervalHours << " ч. Последняя копия: "
                  << lastBackupTime << ".\n";
    }
};

} // namespace software

#endif
