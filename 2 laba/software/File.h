#ifndef FILE_H
#define FILE_H

#include <string>
#include <iostream>

namespace software {

class File {
private:
    std::string fileName;
    size_t fileSizeMB;
    std::string extension;

public:
    File(std::string name, size_t size, std::string ext)
        : fileName(name), fileSizeMB(size), extension(ext) {}

    void displayInfo() const {
        std::cout << "Файл: " << fileName << "." << extension << " (" << fileSizeMB << " МБ)\n";
    }

    std::string getName() const { return fileName; }
    size_t getSize() const { return fileSizeMB; }
    std::string getExtension() const { return extension; }
};

} // namespace software

#endif