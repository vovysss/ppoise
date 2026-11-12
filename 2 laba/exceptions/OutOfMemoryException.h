#ifndef OUT_OF_MEMORY_EXCEPTION_H
#define OUT_OF_MEMORY_EXCEPTION_H

#include <exception>
#include <string>

namespace exceptions {
class OutOfMemoryException : public std::exception {
private:
    std::string message;
public:
    OutOfMemoryException(const std::string& msg) : message("Недостаточно памяти (OOM): " + msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};
}
#endif