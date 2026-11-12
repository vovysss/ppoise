#ifndef BIOS_CORRUPTED_EXCEPTION_H
#define BIOS_CORRUPTED_EXCEPTION_H

#include <exception>
#include <string>

namespace exceptions {
class BIOSCorruptedException : public std::exception {
private:
    std::string message;
public:
    BIOSCorruptedException(const std::string& msg) : message("Ошибка целостности BIOS/UEFI: " + msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};
}
#endif