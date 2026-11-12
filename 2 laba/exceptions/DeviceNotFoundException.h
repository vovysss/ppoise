#ifndef DEVICE_NOT_FOUND_EXCEPTION_H
#define DEVICE_NOT_FOUND_EXCEPTION_H

#include <exception>
#include <string>

namespace exceptions {
class DeviceNotFoundException : public std::exception {
private:
    std::string message;
public:
    DeviceNotFoundException(const std::string& msg) : message("Устройство не найдено: " + msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};
}
#endif