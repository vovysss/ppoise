#ifndef OVERHEATING_EXCEPTION_H
#define OVERHEATING_EXCEPTION_H

#include <exception>
#include <string>

namespace exceptions {

// Наследуемся от стандартного класса std::exception
class OverheatingException : public std::exception {
private:
    std::string message;

public:
    OverheatingException(const std::string& componentName, double currentTemp) {
        message = "Критический перегрев устройства [" + componentName + 
                  "]: температура достигла опасной отметки в " + std::to_string(currentTemp) + "°C!";
    }

    // Переопределяем метод what() для вывода текста ошибки
    const char* what() const noexcept override {
        return message.c_str();
    }
};

} // namespace exceptions

#endif