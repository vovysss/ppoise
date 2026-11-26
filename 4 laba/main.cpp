#include "graph.hpp"
#include "sorting.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

struct Person {
    std::string name;
    int age;

    bool operator<(const Person& other) const {
        if (name != other.name) {
            return name < other.name;
        }
        return age < other.age;
    }
};

struct ByAge {
    bool operator()(const Person& left, const Person& right) const {
        return left.age < right.age;
    }
};

std::ostream& operator<<(std::ostream& out, const Person& person) {
    return out << person.name << '(' << person.age << ')';
}

int read_int(const std::string& prompt, int low, int high) {
    while (true) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) {
            return low;
        }

        try {
            std::size_t used = 0;
            const int value = std::stoi(line, &used);
            if (used == line.size() && value >= low && value <= high) {
                return value;
            }
        } catch (const std::exception&) {
        }

        std::cout << "Введите число от " << low << " до " << high << ".\n";
    }
}

std::string read_text(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

std::vector<int> read_values() {
    std::cout << "Введите целые числа через пробел: ";
    std::string line;
    std::getline(std::cin, line);

    std::vector<int> values;
    std::istringstream input(line);
    int value = 0;
    while (input >> value) {
        values.push_back(value);
    }
    return values;
}

void print_values(const std::vector<int>& values) {
    if (values.empty()) {
        std::cout << "(пусто)";
    }
    for (int value : values) {
        std::cout << value << ' ';
    }
    std::cout << '\n';
}

void demo_sorting() {
    while (true) {
        std::cout << "\n=== Сортировки ===\n"
                  << "1. Smoothsort (std::vector<int>)\n"
                  << "2. Simple pancake sort (std::vector<int>)\n"
                  << "3. Пользовательский класс Person\n"
                  << "4. Smoothsort на массиве (указатели)\n"
                  << "0. Назад\n";

        const int choice = read_int("Выбор: ", 0, 4);
        if (choice == 0) {
            return;
        }

        if (choice == 3) {
            Person people[] = {{"Ivan", 20}, {"Anna", 22}, {"Boris", 19},
                               {"Daria", 21}};

            std::cout << "Исходный массив: ";
            for (const Person& person : people) {
                std::cout << person << ' ';
            }
            std::cout << '\n';

            lab4::smoothsort(std::begin(people), std::end(people));
            std::cout << "Smoothsort по operator<: ";
            for (const Person& person : people) {
                std::cout << person << ' ';
            }
            std::cout << '\n';

            lab4::pancake_sort(std::begin(people), std::end(people), ByAge{});
            std::cout << "Pancake sort по возрасту: ";
            for (const Person& person : people) {
                std::cout << person << ' ';
            }
            std::cout << '\n';
            continue;
        }

        std::vector<int> values = read_values();
        const bool ascending = read_int("1 — по возрастанию, 2 — по убыванию: ",
                                        1, 2) == 1;

        if (choice == 4) {
            constexpr std::size_t capacity = 16;
            int array[capacity] = {};
            const std::size_t count =
                std::min(values.size(), static_cast<std::size_t>(capacity));
            std::copy_n(values.begin(), count, array);

            if (ascending) {
                lab4::smoothsort(array, array + count);
            } else {
                lab4::smoothsort(array, array + count, std::greater<int>());
            }

            std::cout << "Результат: ";
            for (std::size_t i = 0; i < count; ++i) {
                std::cout << array[i] << ' ';
            }
            std::cout << '\n';
            continue;
        }

        if (choice == 1) {
            if (ascending) {
                lab4::smoothsort(values.begin(), values.end());
            } else {
                lab4::smoothsort(values.begin(), values.end(),
                                 std::greater<int>());
            }
        } else {
            if (ascending) {
                lab4::pancake_sort(values.begin(), values.end());
            } else {
                lab4::pancake_sort(values.begin(), values.end(),
                                   std::greater<int>());
            }
        }

        std::cout << "Результат: ";
        print_values(values);
    }
}

void print_vertices(const lab4::graph<std::string>& graph) {
    std::cout << "Вершины: ";
    for (auto it = graph.begin(); it != graph.end(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << "\nВершины в обратном порядке: ";
    for (auto it = graph.rbegin(); it != graph.rend(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << '\n';
}

void print_edges(const lab4::graph<std::string>& graph) {
    std::cout << "Рёбра: ";
    for (auto it = graph.edges_begin(); it != graph.edges_end(); ++it) {
        const auto edge = *it;
        std::cout << '(' << edge.first << ", " << edge.second << ") ";
    }
    std::cout << "\nРёбра в обратном порядке: ";
    for (auto it = graph.edges_rbegin(); it != graph.edges_rend(); ++it) {
        const auto edge = *it;
        std::cout << '(' << edge.first << ", " << edge.second << ") ";
    }
    std::cout << '\n';
}

void demo_graph() {
    lab4::graph<std::string> graph;

    while (true) {
        std::cout << "\n=== Граф (модифицированная структура Вирта) ===\n"
                  << "1. Добавить вершину\n"
                  << "2. Добавить ребро\n"
                  << "3. Удалить вершину\n"
                  << "4. Удалить ребро\n"
                  << "5. Вывести граф\n"
                  << "6. Вершины (итераторы)\n"
                  << "7. Рёбра (итераторы)\n"
                  << "8. Рёбра, инцидентные вершине\n"
                  << "9. Смежные вершины\n"
                  << "10. Степени вершины и ребра\n"
                  << "0. Назад\n";

        const int choice = read_int("Выбор: ", 0, 10);
        if (choice == 0) {
            return;
        }

        try {
            if (choice == 1) {
                const std::string value = read_text("Значение вершины: ");
                graph.add_vertex(value);
                std::cout << "Вершина добавлена.\n";
            } else if (choice == 2) {
                const std::string first = read_text("Первая вершина: ");
                const std::string second = read_text("Вторая вершина: ");
                graph.add_edge(first, second);
                std::cout << "Ребро добавлено.\n";
            } else if (choice == 3) {
                const std::string value = read_text("Значение вершины: ");
                auto it = graph.begin();
                while (it != graph.end() && *it != value) {
                    ++it;
                }
                if (it == graph.end()) {
                    throw std::out_of_range("вершина не найдена");
                }
                graph.erase_vertex(it);
                std::cout << "Вершина удалена вместе с инцидентными рёбрами.\n";
            } else if (choice == 4) {
                const std::string first = read_text("Первая вершина: ");
                const std::string second = read_text("Вторая вершина: ");
                auto it = graph.edges_begin();
                while (it != graph.edges_end()) {
                    const auto edge = *it;
                    if ((edge.first == first && edge.second == second) ||
                        (edge.first == second && edge.second == first)) {
                        break;
                    }
                    ++it;
                }
                if (it == graph.edges_end()) {
                    throw std::out_of_range("ребро не найдено");
                }
                graph.erase_edge(it);
                std::cout << "Ребро удалено.\n";
            } else if (choice == 5) {
                std::cout << graph << '\n';
            } else if (choice == 6) {
                print_vertices(graph);
            } else if (choice == 7) {
                print_edges(graph);
            } else if (choice == 8) {
                const std::string value = read_text("Значение вершины: ");
                auto range = graph.incident_edges(value);
                std::cout << "Инцидентные рёбра: ";
                for (auto it = range.first; it != range.second; ++it) {
                    const auto edge = *it;
                    std::cout << '(' << edge.first << ", " << edge.second
                              << ") ";
                }

                auto reverse_range = graph.reverse_incident_edges(value);
                std::cout << "\nВ обратном порядке: ";
                for (auto it = reverse_range.first;
                     it != reverse_range.second; ++it) {
                    const auto edge = *it;
                    std::cout << '(' << edge.first << ", " << edge.second
                              << ") ";
                }
                std::cout << '\n';
            } else if (choice == 9) {
                const std::string value = read_text("Значение вершины: ");
                auto range = graph.adjacent_vertices(value);
                std::cout << "Смежные вершины: ";
                for (auto it = range.first; it != range.second; ++it) {
                    std::cout << *it << ' ';
                }

                auto reverse_range = graph.reverse_adjacent_vertices(value);
                std::cout << "\nВ обратном порядке: ";
                for (auto it = reverse_range.first;
                     it != reverse_range.second; ++it) {
                    std::cout << *it << ' ';
                }
                std::cout << '\n';
            } else if (choice == 10) {
                const std::string first = read_text("Вершина: ");
                std::cout << "Степень вершины: " << graph.degree(first) << '\n';

                const std::string second =
                    read_text("Вторая вершина ребра: ");
                std::cout << "Степень ребра: "
                          << graph.edge_degree(first, second) << '\n';
            }
        } catch (const std::exception& error) {
            std::cout << "Ошибка: " << error.what() << '\n';
        }
    }
}

} // namespace

int main() {
    while (true) {
        std::cout << "\n=== Лабораторная работа №4 ===\n"
                  << "1. Сортировки\n"
                  << "2. Граф\n"
                  << "0. Выход\n";

        const int choice = read_int("Выбор: ", 0, 2);
        if (choice == 0) {
            break;
        }
        if (choice == 1) {
            demo_sorting();
        } else {
            demo_graph();
        }
    }
    return 0;
}
