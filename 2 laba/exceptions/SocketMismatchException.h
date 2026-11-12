#ifndef SOCKET_MISMATCH_EXCEPTION_H
#define SOCKET_MISMATCH_EXCEPTION_H

#include <exception>
#include <string>

namespace exceptions {
class SocketMismatchException : public std::exception {
private:
    std::string message;
public:
    SocketMismatchException(const std::string& msg) : message("Несовпадение сокета процессора и платы: " + msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};
}
#endif