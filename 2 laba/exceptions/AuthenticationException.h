#ifndef AUTHENTICATION_EXCEPTION_H
#define AUTHENTICATION_EXCEPTION_H

#include <exception>
#include <string>

namespace exceptions {
class AuthenticationException : public std::exception {
private:
    std::string message;
public:
    AuthenticationException(const std::string& msg)
        : message("Ошибка аутентификации: " + msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};
}
#endif
