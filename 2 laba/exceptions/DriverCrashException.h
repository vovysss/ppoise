#ifndef DRIVER_CRASH_EXCEPTION_H
#define DRIVER_CRASH_EXCEPTION_H

#include <exception>
#include <string>

namespace exceptions {
class DriverCrashException : public std::exception {
private:
    std::string message;
public:
    DriverCrashException(const std::string& msg) : message("Сбой драйвера устройства: " + msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};
}
#endif