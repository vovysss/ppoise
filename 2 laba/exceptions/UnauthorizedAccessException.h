#ifndef UNAUTHORIZED_ACCESS_EXCEPTION_H
#define UNAUTHORIZED_ACCESS_EXCEPTION_H

#include <exception>
#include <string>

namespace exceptions {
class UnauthorizedAccessException : public std::exception {
private:
    std::string message;
public:
    UnauthorizedAccessException(const std::string& msg) : message("Несанкционированный доступ: " + msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};
}
#endif