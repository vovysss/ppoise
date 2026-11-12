#ifndef NETWORK_TIMEOUT_EXCEPTION_H
#define NETWORK_TIMEOUT_EXCEPTION_H

#include <exception>
#include <string>

namespace exceptions {
class NetworkTimeoutException : public std::exception {
private:
    std::string message;
public:
    NetworkTimeoutException(const std::string& msg) : message("Тайм-аут сетевого соединения: " + msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};
}
#endif