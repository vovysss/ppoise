#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "File.h"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

namespace software {

class Directory {
private:
    std::string name;
    std::string path;
    std::vector<File> entries;
    Directory* parent;

public:
    Directory(std::string dirName, std::string dirPath, Directory* parentDir = nullptr)
        : name(dirName), path(dirPath), parent(parentDir) {}

    void addFile(const File& file) {
        entries.push_back(file);
        std::cout << "Файл " << file.getName() << " добавлен в каталог " << path << ".\n";
    }

    bool removeFile(const std::string& fileName) {
        auto it = std::find_if(entries.begin(), entries.end(),
            [&fileName](const File& file) { return file.getName() == fileName; });
        if (it == entries.end()) {
            std::cout << "Файл " << fileName << " не найден в " << path << ".\n";
            return false;
        }
        entries.erase(it);
        std::cout << "Файл " << fileName << " удален из каталога " << path << ".\n";
        return true;
    }

    void listContents() const {
        std::cout << "Содержимое каталога " << path;
        if (parent != nullptr) {
            std::cout << " (родительский каталог: " << parent->name << ")";
        }
        std::cout << ":\n" << "Всего файлов: " << entries.size() << ".\n";
        for (const auto& file : entries) {
            file.displayInfo();
        }
    }

    bool findFile(const std::string& fileName) const {
        for (const auto& file : entries) {
            if (file.getName() == fileName) {
                std::cout << "Файл " << fileName << " найден в " << path << ".\n";
                return true;
            }
        }
        std::cout << "Файл " << fileName << " отсутствует в " << path << ".\n";
        return false;
    }
};

} // namespace software

#endif
