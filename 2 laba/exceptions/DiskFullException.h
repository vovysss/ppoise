#ifndef DISK_FULL_EXCEPTION_H
#define DISK_FULL_EXCEPTION_H

#include <exception>
#include <string>

namespace exceptions {
class DiskFullException : public std::exception {
private:
    std::string message;
public:
    DiskFullException(const std::string& msg) : message("Диск переполнен: " + msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};
}
#endif