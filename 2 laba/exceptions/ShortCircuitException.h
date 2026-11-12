#ifndef SHORT_CIRCUIT_EXCEPTION_H
#define SHORT_CIRCUIT_EXCEPTION_H

#include <exception>
#include <string>

namespace exceptions {
class ShortCircuitException : public std::exception {
private:
    std::string message;
public:
    ShortCircuitException(const std::string& msg) : message("Короткое замыкание: " + msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};
}
#endif