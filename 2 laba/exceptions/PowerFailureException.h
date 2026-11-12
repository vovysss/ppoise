#ifndef POWER_FAILURE_EXCEPTION_H
#define POWER_FAILURE_EXCEPTION_H

#include <exception>
#include <string>

namespace exceptions {
class PowerFailureException : public std::exception {
private:
    std::string message;
public:
    PowerFailureException(const std::string& msg) : message("Авария питания БП: " + msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};
}
#endif