
#include <iostream>
#include <limits>
#include "vector.h"

int readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            return value;
        } else {
            std::cout << "Ошибка ввода. Пожалуйста, введите целое число.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

int main() {
    Vector a, b;
    bool hasA = false, hasB = false;

    while (true) {
        std::cout << "\n===== ВЕКТОР =====\n";
        std::cout << "1. Ввести вектор A\n";
        std::cout << "2. Ввести вектор B\n";
        std::cout << "3. Показать A и B\n";
        std::cout << "4. Длина A\n";
        std::cout << "5. A + B\n";
        std::cout << "6. A - B\n";
        std::cout << "7. A * B (векторное произведение)\n";
        std::cout << "8. A * скаляр\n";
        std::cout << "9. A / скаляр\n";
        std::cout << "10. Косинус угла между A и B\n";
        std::cout << "11. Сравнить A и B по длине\n";
        std::cout << "0. Выход\n";
        
        int choice = readInt("Выбор: ");

        try {
            switch (choice) {
                case 1:
                    std::cin >> a;
                    hasA = true;
                    std::cout << "Вектор A успешно введен.\n";
                    break;
                case 2:
                    std::cin >> b;
                    hasB = true;
                    std::cout << "Вектор B успешно введен.\n";
                    break;
                case 3:
                    if (hasA) std::cout << "A: " << a << "\n";
                    else std::cout << "Вектор A не задан.\n";
                    if (hasB) std::cout << "B: " << b << "\n";
                    else std::cout << "Вектор B не задан.\n";
                    break;
                case 4:
                    if (hasA) {
                        std::cout << "Длина A = " << a.length() << "\n";
                    } else {
                        std::cout << "Сначала введите вектор A!\n";
                    }
                    break;
                case 5:
                    if (hasA && hasB) {
                        std::cout << "A + B = " << (a + b) << "\n";
                    } else {
                        std::cout << "Необходимо ввести оба вектора A и B!\n";
                    }
                    break;
                case 6:
                    if (hasA && hasB) {
                        std::cout << "A - B = " << (a - b) << "\n";
                    } else {
                        std::cout << "Необходимо ввести оба вектора A и B!\n";
                    }
                    break;
                case 7:
                    if (hasA && hasB) {
                        std::cout << "A * B (векторное произведение) = " << (a * b) << "\n";
                    } else {
                        std::cout << "Необходимо ввести оба вектора A и B!\n";
                    }
                    break;
                case 8: {
                    if (!hasA) {
                        std::cout << "Сначала введите вектор A!\n";
                        break;
                    }
                    double scalar;
                    std::cout << "Введите скаляр: ";
                    std::cin >> scalar;
                    std::cout << "A * " << scalar << " = " << (a * scalar) << "\n";
                    break;
                }
                case 9: {
                    if (!hasA) {
                        std::cout << "Сначала введите вектор A!\n";
                        break;
                    }
                    double scalar;
                    std::cout << "Введите скаляр: ";
                    std::cin >> scalar;
                    std::cout << "A / " << scalar << " = " << (a / scalar) << "\n";
                    break;
                }
                case 10:
                    if (hasA && hasB) {
                        std::cout << "Косинус угла между A и B = " << (a ^ b) << "\n";
                    } else {
                        std::cout << "Необходимо ввести оба вектора A и B!\n";
                    }
                    break;
                case 11:
                    if (hasA && hasB) {
                        std::cout << "Результаты сравнения по длине:\n";
                        std::cout << "A == B: " << (a == b ? "Да" : "Нет") << "\n";
                        std::cout << "A != B: " << (a != b ? "Да" : "Нет") << "\n";
                        std::cout << "A > B (длина A > длина B): " << (a > b ? "Да" : "Нет") << "\n";
                        std::cout << "A < B (длина A < длина B): " << (a < b ? "Да" : "Нет") << "\n";
                    } else {
                        std::cout << "Необходимо ввести оба вектора A и B!\n";
                    }
                    break;
                case 0:
                    std::cout << "Выход из программы.\n";
                    return 0;
                default:
                    std::cout << "Неверный выбор. Попробуйте снова.\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << "\n";
        }
    }
    return 0;
}