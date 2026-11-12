#ifndef INVALID_CONFIGURATION_EXCEPTION_H
#define INVALID_CONFIGURATION_EXCEPTION_H

#include <exception>
#include <string>

namespace exceptions {
class InvalidConfigurationException : public std::exception {
private:
    std::string message;
public:
    InvalidConfigurationException(const std::string& msg)
        : message("Некорректная конфигурация системы: " + msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};
}
#endif
