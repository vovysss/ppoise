# Лабораторная работа №4. Обобщённое программирование: сортировка и контейнеры

C++17, только стандартная библиотека STL. Вариант сортировки — **13**
(Smoothsort и Simple pancake sort), вариант графа — **2.1.13**
(неориентированный граф, модифицированная структура Вирта).

## Структура проекта

```
.
├── CMakeLists.txt
├── include/
│   ├── sorting.hpp      # обобщённые сортировки
│   └── graph.hpp        # шаблонный класс графа
├── src/
│   └── main.cpp         # консольное меню
└── tests/
    └── test_main.cpp    # unit-тесты без сторонних библиотек
```

## Часть 1. Сортировки

В `include/sorting.hpp` реализованы:

- `lab4::smoothsort(first, last[, comp])` — Smoothsort Дейкстры на числах
  Леонардо;
- `lab4::pancake_sort(first, last[, comp])` — простой вариант pancake sort
  (два переворота префикса на каждом шаге).

Обе функции работают с произвольными random-access диапазонами: обычными
массивами (через указатели), `std::begin/std::end` и итераторами
`std::vector`. Поддерживаются встроенные типы, пользовательские классы с
`operator<` и пользовательские компараторы.

Демонстрационный класс `Person` определён в CLI и тестах. В CLI показана
сортировка как по `operator<`, так и по компаратору `ByAge`.

## Часть 2. Граф

Шаблон `lab4::graph<T>` скрывает внутреннее представление:

- вершины хранятся в двусвязном списке;
- у каждой вершины есть двусвязный список инцидентностей;
- рёбра хранятся в общем двусвязном списке.

Отдельных классов-обёрток для вершин и рёбер нет: ребро представляется
`std::pair<T, T>`. Граф простой: петли и кратные рёбра запрещены.

### Публичные типы

```cpp
using value_type  = T;
using edge_type   = std::pair<T, T>;
using size_type   = std::size_t;
using difference_type = std::ptrdiff_t;

// Вершины
using vertex_iterator;
using const_vertex_iterator;
using reverse_vertex_iterator;
using const_reverse_vertex_iterator;

// Рёбра
using edge_iterator;
using const_edge_iterator;
using reverse_edge_iterator;
using const_reverse_edge_iterator;

// Рёбра, инцидентные вершине
using incident_edge_iterator;
using const_incident_edge_iterator;
using reverse_incident_edge_iterator;
using const_reverse_incident_edge_iterator;

// Смежные вершины
using adjacent_vertex_iterator;
using const_adjacent_vertex_iterator;
using reverse_adjacent_vertex_iterator;
using const_reverse_adjacent_vertex_iterator;
```

### Основные методы

```cpp
graph();                              // пустой граф
graph(const graph&);                  // копирование
~graph();                             // освобождение памяти
graph& operator=(const graph&);       // присваивание копированием

bool empty() const;
void clear();

bool has_vertex(const T&) const;
bool has_edge(const T&, const T&) const;
size_type vertex_count() const;
size_type edge_count() const;
size_type degree(const T&) const;
size_type edge_degree(const T&, const T&) const;
size_type edge_degree(const edge_type&) const;

vertex_iterator add_vertex(const T&);
edge_iterator add_edge(const T&, const T&);

void erase_vertex(vertex_iterator);
void erase_edge(edge_iterator);

// Обход вершин
vertex_iterator begin();
vertex_iterator end();
reverse_vertex_iterator rbegin();
reverse_vertex_iterator rend();
// + const-версии, cbegin/cend, crbegin/crend

// Обход рёбер
edge_iterator edges_begin();
edge_iterator edges_end();
reverse_edge_iterator edges_rbegin();
reverse_edge_iterator edges_rend();
// + const-версии, edges_cbegin/edges_cend,
//   edges_crbegin/edges_crend

// Рёбра, инцидентные вершине
std::pair<incident_edge_iterator, incident_edge_iterator>
incident_edges(const T&);
std::pair<reverse_incident_edge_iterator, reverse_incident_edge_iterator>
reverse_incident_edges(const T&);
// + const-версии

// Смежные вершины
std::pair<adjacent_vertex_iterator, adjacent_vertex_iterator>
adjacent_vertices(const T&);
std::pair<reverse_adjacent_vertex_iterator, reverse_adjacent_vertex_iterator>
reverse_adjacent_vertices(const T&);
// + const-версии
```

Итераторы двунаправленные: поддерживают `*`, `->` (там, где применимо),
`++`, `--`, постфиксные формы и сравнение. Для всех четырёх видов обхода
есть обычные, константные и обратные варианты.

### Сравнение и вывод

Для графа определены `==`, `!=`, `<`, `<=`, `>`, `>=`.
Сравнение не зависит от порядка добавления вершин и рёбер: вершины и рёбра
канонизируются. `operator<<` использует итераторы и `std::for_each`:

```cpp
graph{vertices:[1, 2, 3], edges:[(1, 2), (2, 3)]}
```

### Исключения

- `std::invalid_argument` — повторная вершина, повторное ребро, петля,
  неверный итератор;
- `std::out_of_range` — отсутствующая вершина или ребро.

## Сборка и запуск

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
./build/lab4
```

Запуск тестов:

```bash
ctest --test-dir build --output-on-failure
```

## Покрытие

Тесты находятся в `tests/test_main.cpp` и не требуют сторонних фреймворков.
Для сборки с инструментами покрытия:

```bash
cmake -S . -B build-cov -DLAB4_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build-cov -j
./build-cov/lab4_tests
cd build-cov/CMakeFiles/lab4_tests.dir/tests
gcov -b -c test_main.cpp.gcno
```

Текущее покрытие строк:

- `include/sorting.hpp` — 100%;
- `include/graph.hpp` — около 97.6%.

Всего выполняется 284 проверки.
