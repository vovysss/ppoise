#include <iostream>
#include "rectangle.h"

static void printMenu() {
    std::cout << "\n Меню \n"
              << " 1. Ввести прямоугольник A\n"
              << " 2. Ввести прямоугольник B\n"
              << " 3. Показать A и B\n"
              << " 4. Переместить A\n"
              << " 5. Изменить размер A\n"
              << " 6. ++A (префикс)\n"
              << " 7. A++ (постфикс)\n"
              << " 8. --A (префикс)\n"
              << " 9. A-- (постфикс)\n"
              << "10. C = A + B  (объединение)\n"
              << "11. C = A - B  (пересечение)\n"
              << "12. A += B\n"
              << "13. A -= B\n"
              << "14. A == B ?\n"
              << "15. Пересекаются ли A и B?\n"
              << "16. Показать площадь A\n"
              << " 0. Выход\n"
              << "Выбор: ";
}

int main() {
    Rectangle A, B, C;
    int choice = -1;

    while (choice != 0) {
        printMenu();
        if (!(std::cin >> choice)) break;

        switch (choice) {
            case 1:
                std::cout << "\n--- Ввод прямоугольника A ---\n";
                std::cin >> A;
                break;

            case 2:
                std::cout << "\n--- Ввод прямоугольника B ---\n";
                std::cin >> B;
                break;

            case 3:
                std::cout << "\n--- Текущие прямоугольники ---\n";
                std::cout << "A: " << A << "\n";
                std::cout << "B: " << B << "\n";
                break;

            case 4: {
                int dx, dy;
                std::cout << "Введите смещение dx и dy: ";
                std::cin >> dx >> dy;
                A.move(dx, dy);
                std::cout << "Прямоугольник A перемещен. Новое состояние: " << A << "\n";
                break;
            }

            case 5: {
                int w, h;
                std::cout << "Введите новую ширину и высоту: ";
                std::cin >> w >> h;
                A.resize(w, h);
                std::cout << "Размер A изменен. Новое состояние: " << A << "\n";
                break;
            }

            case 6:
                ++A;
                std::cout << "После ++A: " << A << "\n";
                break;

            case 7:
                A++;
                std::cout << "После A++: " << A << "\n";
                break;

            case 8:
                --A;
                std::cout << "После --A: " << A << "\n";
                break;

            case 9:
                A--;
                std::cout << "После A--: " << A << "\n";
                break;

            case 10:
                C = A + B;
                std::cout << "C = A + B (объединение): " << C << "\n";
                break;

            case 11:
                C = A - B;
                std::cout << "C = A - B (пересечение): " << C << "\n";
                break;

            case 12:
                A += B;
                std::cout << "После A += B: " << A << "\n";
                break;

            case 13:
                A -= B;
                std::cout << "После A -= B: " << A << "\n";
                break;

            case 14:
                if (A == B) {
                    std::cout << "Прямоугольники A и B равны.\n";
                } else {
                    std::cout << "Прямоугольники A и B НЕ равны.\n";
                }
                break;

            case 15:
                if (A.intersects(B)) {
                    std::cout << "Прямоугольники A и B пересекаются.\n";
                } else {
                    std::cout << "Прямоугольники A и B НЕ пересекаются.\n";
                }
                break;

            case 16:
                std::cout << "Площадь прямоугольника A = " << A.area() << "\n";
                break;

            case 0:
                std::cout << "Выход из программы.\n";
                break;

            default:
                std::cout << "Неверный выбор. Попробуйте снова.\n";
        }
    }

    return 0;
}