#ifndef COMPILER_H
#define COMPILER_H

#include "File.h"
#include <vector>
#include <string>
#include <iostream>

namespace software {

class Compiler {
private:
    std::string language;
    int optimizationLevel;
    int errorCount;
    int warningCount;

public:
    Compiler(std::string targetLanguage, int optimization)
        : language(targetLanguage), optimizationLevel(optimization),
          errorCount(0), warningCount(0) {}

    bool compile(const File& sourceFile) {
        if (sourceFile.getExtension() != "cpp" && sourceFile.getExtension() != "c") {
            ++errorCount;
            std::cout << "Ошибка компиляции: файл " << sourceFile.getName()
                      << " не является исходником " << language << ".\n";
            return false;
        }
        std::cout << "Файл " << sourceFile.getName() << " скомпилирован ("
                  << language << ", -O" << optimizationLevel << ").\n";
        return true;
    }

    void link(const std::vector<File>& objectFiles) {
        std::cout << "Компоновка " << objectFiles.size() << " объектных файлов...\n";
        if (objectFiles.empty()) {
            ++errorCount;
            std::cout << "Ошибка компоновки: нет объектных файлов.\n";
        } else {
            std::cout << "Исполняемый файл успешно скомпонован.\n";
        }
    }

    void reportErrors() const {
        std::cout << "Отчет компилятора: ошибок " << errorCount
                  << ", предупреждений " << warningCount << ".\n";
    }

    void optimize(int level) {
        optimizationLevel = level;
        std::cout << "Уровень оптимизации компилятора изменен на -O"
                  << optimizationLevel << ".\n";
    }
};

} // namespace software

#endif
