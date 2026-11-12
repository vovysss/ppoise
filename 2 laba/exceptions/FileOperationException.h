#ifndef FILE_OPERATION_EXCEPTION_H
#define FILE_OPERATION_EXCEPTION_H

#include <exception>
#include <string>

namespace exceptions {
class FileOperationException : public std::exception {
private:
    std::string message;
public:
    FileOperationException(const std::string& msg)
        : message("Ошибка файловой операции: " + msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};
}
#endif
