#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include "Directory.h"
#include "File.h"
#include <vector>
#include <string>
#include <iostream>

namespace software {

class FileSystem {
private:
    std::string rootPath;
    double totalSpaceGB;
    double usedSpaceGB;
    std::vector<Directory> directories;
    std::vector<File> files;

public:
    FileSystem(std::string root, double totalSpace)
        : rootPath(root), totalSpaceGB(totalSpace), usedSpaceGB(0.0) {}

    void createFile(const File& file) {
        double fileSizeGB = file.getSize() / 1024.0;
        if (usedSpaceGB + fileSizeGB > totalSpaceGB) {
            std::cout << "Недостаточно места в файловой системе " << rootPath << ".\n";
            return;
        }
        files.push_back(file);
        usedSpaceGB += fileSizeGB;
        std::cout << "Файл " << file.getName() << " создан в " << rootPath
                  << ". Занято " << usedSpaceGB << " ГБ.\n";
    }

    bool deleteFile(const std::string& fileName) {
        for (auto it = files.begin(); it != files.end(); ++it) {
            if (it->getName() == fileName) {
                usedSpaceGB -= it->getSize() / 1024.0;
                files.erase(it);
                std::cout << "Файл " << fileName << " удален из " << rootPath << ".\n";
                return true;
            }
        }
        std::cout << "Файл " << fileName << " не найден в " << rootPath << ".\n";
        return false;
    }

    void format() {
        files.clear();
        directories.clear();
        usedSpaceGB = 0.0;
        std::cout << "Файловая система " << rootPath << " отформатирована.\n";
    }

    bool checkIntegrity() const {
        bool ok = usedSpaceGB <= totalSpaceGB;
        std::cout << "Проверка целостности " << rootPath << ": "
                  << (ok ? "ошибок нет." : "обнаружено переполнение.") << "\n";
        return ok;
    }
};

} // namespace software

#endif
