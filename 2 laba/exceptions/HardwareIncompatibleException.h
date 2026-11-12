#ifndef HARDWARE_INCOMPATIBLE_EXCEPTION_H
#define HARDWARE_INCOMPATIBLE_EXCEPTION_H

#include <exception>
#include <string>

namespace exceptions {
class HardwareIncompatibleException : public std::exception {
private:
    std::string message;
public:
    HardwareIncompatibleException(const std::string& msg) : message("Аппаратная несовместимость компонентов: " + msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};
}
#endif