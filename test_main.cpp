#include "graph.hpp"
#include "sorting.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {

int checks = 0;
int failures = 0;

void report_failure(const char* text, const char* file, int line) {
    ++failures;
    std::cerr << file << ':' << line << ": CHECK failed: " << text << '\n';
}

template <class Exception, class Function>
void check_throws(const char* text, const char* file, int line,
                  Function&& function) {
    ++checks;
    try {
        function();
    } catch (const Exception&) {
        return;
    } catch (const std::exception& error) {
        ++failures;
        std::cerr << file << ':' << line << ": CHECK_THROW failed: " << text
                  << " (wrong exception: " << error.what() << ")\n";
        return;
    } catch (...) {
        ++failures;
        std::cerr << file << ':' << line << ": CHECK_THROW failed: " << text
                  << " (unknown exception)\n";
        return;
    }

    ++failures;
    std::cerr << file << ':' << line << ": CHECK_THROW failed: " << text
              << " (no exception)\n";
}

#define CHECK(...)                                                            \
    do {                                                                      \
        ++checks;                                                             \
        if (!(__VA_ARGS__)) {                                                 \
            report_failure(#__VA_ARGS__, __FILE__, __LINE__);                 \
        }                                                                     \
    } while (false)

#define CHECK_THROW(expression, exception)                                    \
    check_throws<exception>(#expression, __FILE__, __LINE__,                  \
                            [&] { expression; })

struct Person {
    std::string name;
    int age;

    bool operator<(const Person& other) const {
        if (name != other.name) {
            return name < other.name;
        }
        return age < other.age;
    }

    bool operator==(const Person& other) const {
        return name == other.name && age == other.age;
    }
};

struct ByAge {
    bool operator()(const Person& left, const Person& right) const {
        return left.age < right.age;
    }
};

struct ThrowingValue {
    inline static int copies_before_throw = -1;

    int value = 0;

    ThrowingValue() = default;
    explicit ThrowingValue(int number) : value(number) {}

    ThrowingValue(const ThrowingValue& other) : value(other.value) {
        if (copies_before_throw == 0) {
            throw std::runtime_error("copy failed");
        }
        if (copies_before_throw > 0) {
            --copies_before_throw;
        }
    }

    ThrowingValue& operator=(const ThrowingValue&) = default;

    bool operator==(const ThrowingValue& other) const {
        return value == other.value;
    }
};

template <class Container, class Compare>
void check_sorted(const Container& values, Compare comp,
                  const char* message) {
    ++checks;
    if (!std::is_sorted(values.begin(), values.end(), comp)) {
        ++failures;
        std::cerr << "sorting failed: " << message << '\n';
    }
}

void test_smoothsort() {
    {
        std::vector<int> values;
        lab4::smoothsort(values.begin(), values.end());
        CHECK(values.empty());

        values.push_back(42);
        lab4::smoothsort(values.begin(), values.end());
        CHECK(values.size() == 1 && values[0] == 42);
    }

    {
        int values[] = {5, -1, 4, 0, 2, 9, 2};
        lab4::smoothsort(std::begin(values), std::end(values));
        CHECK(std::is_sorted(std::begin(values), std::end(values)));
    }

    {
        int values[] = {5, -1, 4, 0, 2, 9, 2};
        lab4::smoothsort(values, values + 7, std::greater<int>());
        CHECK(std::is_sorted(values, values + 7, std::greater<int>()));
    }

    {
        std::vector<std::string> values{"pear", "apple", "orange", "apple"};
        lab4::smoothsort(values.begin(), values.end());
        check_sorted(values, std::less<std::string>(), "smoothsort strings");
    }

    {
        std::vector<double> values{3.5, -2.0, 7.25, 0.0, 1.5};
        lab4::smoothsort(values.begin(), values.end(),
                         [](double left, double right) { return left > right; });
        check_sorted(values, [](double left, double right) { return left > right; },
                     "smoothsort doubles descending");
    }

    {
        std::vector<int> values{1, 2, 3, 4, 5, 6, 7, 8, 9};
        lab4::smoothsort(values.begin(), values.end());
        check_sorted(values, std::less<int>(), "smoothsort already sorted");
    }

    {
        std::vector<int> values{9, 8, 7, 6, 5, 4, 3, 2, 1};
        lab4::smoothsort(values.begin(), values.end());
        check_sorted(values, std::less<int>(), "smoothsort reversed");
    }

    {
        std::vector<int> values(25, 7);
        lab4::smoothsort(values.begin(), values.end());
        check_sorted(values, std::less<int>(), "smoothsort equal values");
    }

    std::mt19937 generator(123456u);
    for (std::size_t size : {0u, 1u, 2u, 3u, 4u, 5u, 8u, 13u, 21u, 50u,
                             100u, 500u, 1000u}) {
        std::vector<int> source(size);
        for (int& value : source) {
            value = static_cast<int>(generator() % 200u) - 100;
        }

        std::vector<int> expected = source;
        std::sort(expected.begin(), expected.end());

        std::vector<int> actual = source;
        lab4::smoothsort(actual.begin(), actual.end());
        CHECK(actual == expected);

        std::sort(expected.begin(), expected.end(), std::greater<int>());
        actual = source;
        lab4::smoothsort(actual.begin(), actual.end(), std::greater<int>());
        CHECK(actual == expected);
    }

    {
        Person people[] = {{"Ivan", 20}, {"Anna", 22}, {"Boris", 19}};
        lab4::smoothsort(std::begin(people), std::end(people));
        CHECK(std::is_sorted(std::begin(people), std::end(people)));
        CHECK(people[0].name == "Anna");
        CHECK(people[1].name == "Boris");
        CHECK(people[2].name == "Ivan");
    }

    {
        Person people[] = {{"Ivan", 20}, {"Anna", 22}, {"Boris", 19}};
        lab4::smoothsort(std::begin(people), std::end(people), ByAge{});
        CHECK(std::is_sorted(std::begin(people), std::end(people), ByAge{}));
        CHECK(people[0].age == 19);
        CHECK(people[2].age == 22);
    }

    {
        std::vector<Person> people{{"Ivan", 20}, {"Anna", 22}, {"Boris", 19}};
        lab4::smoothsort(people.begin(), people.end(), ByAge{});
        check_sorted(people, ByAge{}, "smoothsort Person by age");
    }
}

void test_pancake_sort() {
    {
        std::vector<int> values;
        lab4::pancake_sort(values.begin(), values.end());
        CHECK(values.empty());

        values.push_back(42);
        lab4::pancake_sort(values.begin(), values.end());
        CHECK(values.size() == 1 && values[0] == 42);
    }

    {
        int values[] = {5, -1, 4, 0, 2, 9, 2};
        lab4::pancake_sort(std::begin(values), std::end(values));
        CHECK(std::is_sorted(std::begin(values), std::end(values)));
    }

    {
        int values[] = {5, -1, 4, 0, 2, 9, 2};
        lab4::pancake_sort(values, values + 7, std::greater<int>());
        CHECK(std::is_sorted(values, values + 7, std::greater<int>()));
    }

    {
        std::vector<std::string> values{"pear", "apple", "orange", "apple"};
        lab4::pancake_sort(values.begin(), values.end());
        check_sorted(values, std::less<std::string>(), "pancake strings");
    }

    {
        std::vector<int> values{1, 2, 3, 4, 5, 6, 7, 8, 9};
        lab4::pancake_sort(values.begin(), values.end());
        check_sorted(values, std::less<int>(), "pancake already sorted");
    }

    {
        std::vector<int> values{9, 8, 7, 6, 5, 4, 3, 2, 1};
        lab4::pancake_sort(values.begin(), values.end());
        check_sorted(values, std::less<int>(), "pancake reversed");
    }

    {
        std::vector<int> values(20, 3);
        lab4::pancake_sort(values.begin(), values.end());
        check_sorted(values, std::less<int>(), "pancake equal values");
    }

    std::mt19937 generator(654321u);
    for (std::size_t size : {0u, 1u, 2u, 3u, 5u, 10u, 25u, 100u, 250u}) {
        std::vector<int> source(size);
        for (int& value : source) {
            value = static_cast<int>(generator() % 100u);
        }

        std::vector<int> expected = source;
        std::sort(expected.begin(), expected.end());

        std::vector<int> actual = source;
        lab4::pancake_sort(actual.begin(), actual.end());
        CHECK(actual == expected);

        std::sort(expected.begin(), expected.end(), std::greater<int>());
        actual = source;
        lab4::pancake_sort(actual.begin(), actual.end(), std::greater<int>());
        CHECK(actual == expected);
    }

    {
        Person people[] = {{"Ivan", 20}, {"Anna", 22}, {"Boris", 19}};
        lab4::pancake_sort(std::begin(people), std::end(people));
        CHECK(std::is_sorted(std::begin(people), std::end(people)));

        lab4::pancake_sort(std::begin(people), std::end(people), ByAge{});
        CHECK(std::is_sorted(std::begin(people), std::end(people), ByAge{}));
    }

    {
        std::vector<Person> people{{"Ivan", 20}, {"Anna", 22}, {"Boris", 19}};
        lab4::pancake_sort(people.begin(), people.end(), ByAge{});
        check_sorted(people, ByAge{}, "pancake Person by age");
    }
}

template <class Graph>
typename Graph::vertex_iterator find_vertex(Graph& graph,
                                             const typename Graph::value_type& value) {
    typename Graph::vertex_iterator it = graph.begin();
    while (it != graph.end() && *it != value) {
        ++it;
    }
    return it;
}

template <class Graph>
typename Graph::edge_iterator find_edge(Graph& graph,
                                        const typename Graph::value_type& first,
                                        const typename Graph::value_type& second) {
    typename Graph::edge_iterator it = graph.edges_begin();
    while (it != graph.edges_end()) {
        typename Graph::edge_type edge = *it;
        if ((edge.first == first && edge.second == second) ||
            (edge.first == second && edge.second == first)) {
            return it;
        }
        ++it;
    }
    return it;
}

void test_graph_basics() {
    lab4::graph<int> graph;
    CHECK(graph.empty());
    CHECK(graph.vertex_count() == 0);
    CHECK(graph.edge_count() == 0);
    CHECK(!graph.has_vertex(1));
    CHECK(!graph.has_edge(1, 2));
    CHECK_THROW(graph.degree(1), std::out_of_range);
    CHECK_THROW(graph.edge_degree(1, 2), std::out_of_range);
    CHECK_THROW(graph.edge_degree(lab4::graph<int>::edge_type(1, 2)),
                std::out_of_range);

    auto vertex = graph.add_vertex(1);
    CHECK(*vertex == 1);
    graph.add_vertex(2);
    graph.add_vertex(3);
    CHECK(!graph.empty());
    CHECK(graph.vertex_count() == 3);
    CHECK(graph.has_vertex(1));
    CHECK(!graph.has_vertex(42));
    CHECK_THROW(graph.add_vertex(2), std::invalid_argument);
    CHECK(graph.vertex_count() == 3);

    auto edge = graph.add_edge(1, 2);
    CHECK((*edge).first == 1);
    CHECK((*edge).second == 2);
    CHECK(graph.edge_count() == 1);
    CHECK(graph.has_edge(1, 2));
    CHECK(graph.has_edge(2, 1));
    CHECK(!graph.has_edge(1, 3));
    CHECK_THROW(graph.add_edge(1, 2), std::invalid_argument);
    CHECK_THROW(graph.add_edge(2, 1), std::invalid_argument);
    CHECK_THROW(graph.add_edge(1, 1), std::invalid_argument);
    CHECK_THROW(graph.add_edge(99, 99), std::invalid_argument);
    CHECK_THROW(graph.add_edge(1, 99), std::out_of_range);
    CHECK_THROW(graph.add_edge(99, 1), std::out_of_range);
    CHECK_THROW(graph.add_edge(99, 98), std::out_of_range);
    CHECK(graph.edge_count() == 1);

    graph.add_edge(2, 3);
    graph.add_edge(1, 3);
    CHECK(graph.edge_count() == 3);
    CHECK(graph.degree(1) == 2);
    CHECK(graph.degree(2) == 2);
    CHECK(graph.degree(3) == 2);
    CHECK(graph.edge_degree(1, 2) == 2);
    CHECK(graph.edge_degree({3, 2}) == 2);
    CHECK_THROW(graph.degree(4), std::out_of_range);
    CHECK_THROW(graph.edge_degree(1, 4), std::out_of_range);

    graph.clear();
    CHECK(graph.empty());
    CHECK(graph.vertex_count() == 0);
    CHECK(graph.edge_count() == 0);
    CHECK(!graph.has_vertex(1));
    graph.clear();
    CHECK(graph.empty());
}

void test_graph_iterators() {
    lab4::graph<std::string> graph;
    graph.add_vertex("a");
    graph.add_vertex("b");
    graph.add_vertex("c");
    graph.add_vertex("d");
    graph.add_edge("a", "b");
    graph.add_edge("b", "c");
    graph.add_edge("a", "c");

    CHECK(std::distance(graph.begin(), graph.end()) == 4);
    CHECK(std::distance(graph.rbegin(), graph.rend()) == 4);
    CHECK(std::distance(graph.edges_begin(), graph.edges_end()) == 3);
    CHECK(std::distance(graph.edges_rbegin(), graph.edges_rend()) == 3);

    {
        std::vector<std::string> values;
        for (auto it = graph.begin(); it != graph.end(); ++it) {
            values.push_back(*it);
        }
        CHECK(values == std::vector<std::string>({"a", "b", "c", "d"}));
    }

    {
        auto it = graph.begin();
        CHECK(it->size() == 1);
        auto old = it++;
        CHECK(*old == "a");
        CHECK(*it == "b");
        auto old2 = it--;
        CHECK(*old2 == "b");
        CHECK(*it == "a");

        auto last = graph.end();
        --last;
        CHECK(*last == "d");
        CHECK(last == --graph.end());

        lab4::graph<std::string>::vertex_iterator empty;
        CHECK(empty == lab4::graph<std::string>::vertex_iterator());
        CHECK(empty != graph.begin());

        lab4::graph<std::string>::const_vertex_iterator converted = graph.begin();
        CHECK(*converted == "a");
    }

    {
        std::vector<std::string> values;
        for (auto it = graph.rbegin(); it != graph.rend(); ++it) {
            values.push_back(*it);
        }
        CHECK(values == std::vector<std::string>({"d", "c", "b", "a"}));
    }

    {
        *graph.begin() = "aa";
        CHECK(graph.has_vertex("aa"));
        CHECK(!graph.has_vertex("a"));
        CHECK(graph.has_edge("aa", "b"));
        *graph.begin() = "a";
        CHECK(graph.has_vertex("a"));
    }

    {
        std::vector<std::pair<std::string, std::string>> edges;
        auto it = graph.edges_begin();
        auto old = it++;
        CHECK(*old == std::make_pair(std::string("a"), std::string("b")));
        CHECK(*it == std::make_pair(std::string("b"), std::string("c")));
        auto old2 = it--;
        CHECK(*old2 == std::make_pair(std::string("b"), std::string("c")));

        for (; it != graph.edges_end(); ++it) {
            edges.push_back(*it);
        }
        CHECK(edges ==
              std::vector<std::pair<std::string, std::string>>(
                  {{"a", "b"}, {"b", "c"}, {"a", "c"}}));

        auto last = graph.edges_end();
        --last;
        CHECK(*last == std::make_pair(std::string("a"), std::string("c")));

        lab4::graph<std::string>::edge_iterator empty;
        CHECK(empty == lab4::graph<std::string>::edge_iterator());
        CHECK(empty != graph.edges_begin());
    }

    {
        std::vector<std::pair<std::string, std::string>> edges;
        for (auto it = graph.edges_rbegin(); it != graph.edges_rend(); ++it) {
            edges.push_back(*it);
        }
        CHECK(edges ==
              std::vector<std::pair<std::string, std::string>>(
                  {{"a", "c"}, {"b", "c"}, {"a", "b"}}));
    }

    {
        auto range = graph.incident_edges("b");
        std::vector<std::pair<std::string, std::string>> edges;
        for (auto it = range.first; it != range.second; ++it) {
            edges.push_back(*it);
        }
        CHECK(edges ==
              std::vector<std::pair<std::string, std::string>>(
                  {{"a", "b"}, {"b", "c"}}));

        auto it = range.first;
        auto old = it++;
        CHECK(*old == std::make_pair(std::string("a"), std::string("b")));
        CHECK(*it == std::make_pair(std::string("b"), std::string("c")));
        auto old2 = it--;
        CHECK(*old2 == std::make_pair(std::string("b"), std::string("c")));

        auto last = range.second;
        --last;
        CHECK(*last == std::make_pair(std::string("b"), std::string("c")));

        lab4::graph<std::string>::const_incident_edge_iterator converted =
            range.first;
        CHECK(*converted == std::make_pair(std::string("a"), std::string("b")));

        lab4::graph<std::string>::reverse_incident_edge_iterator rbegin(
            range.second);
        lab4::graph<std::string>::reverse_incident_edge_iterator rend(
            range.first);
        std::vector<std::pair<std::string, std::string>> reversed;
        for (; rbegin != rend; ++rbegin) {
            reversed.push_back(*rbegin);
        }
        CHECK(reversed ==
              std::vector<std::pair<std::string, std::string>>(
                  {{"b", "c"}, {"a", "b"}}));

        auto reverse_range = graph.reverse_incident_edges("b");
        std::vector<std::pair<std::string, std::string>> reversed2;
        for (auto it = reverse_range.first; it != reverse_range.second; ++it) {
            reversed2.push_back(*it);
        }
        CHECK(reversed2 == reversed);
    }

    {
        auto range = graph.adjacent_vertices("b");
        std::vector<std::string> values;
        for (auto it = range.first; it != range.second; ++it) {
            values.push_back(*it);
        }
        CHECK(values == std::vector<std::string>({"a", "c"}));
        CHECK(range.first->size() == 1);

        auto it = range.first;
        auto old = it++;
        CHECK(*old == "a");
        CHECK(*it == "c");
        auto old2 = it--;
        CHECK(*old2 == "c");

        auto last = range.second;
        --last;
        CHECK(*last == "c");

        lab4::graph<std::string>::const_adjacent_vertex_iterator converted =
            range.first;
        CHECK(*converted == "a");

        lab4::graph<std::string>::reverse_adjacent_vertex_iterator rbegin(
            range.second);
        lab4::graph<std::string>::reverse_adjacent_vertex_iterator rend(
            range.first);
        std::vector<std::string> reversed;
        for (; rbegin != rend; ++rbegin) {
            reversed.push_back(*rbegin);
        }
        CHECK(reversed == std::vector<std::string>({"c", "a"}));

        auto reverse_range = graph.reverse_adjacent_vertices("b");
        std::vector<std::string> reversed2;
        for (auto it = reverse_range.first; it != reverse_range.second; ++it) {
            reversed2.push_back(*it);
        }
        CHECK(reversed2 == reversed);
    }

    {
        auto incident = graph.incident_edges("d");
        CHECK(incident.first == incident.second);

        auto adjacent = graph.adjacent_vertices("d");
        CHECK(adjacent.first == adjacent.second);

        lab4::graph<std::string>::reverse_incident_edge_iterator irbegin(
            incident.second);
        lab4::graph<std::string>::reverse_incident_edge_iterator irend(
            incident.first);
        CHECK(irbegin == irend);

        lab4::graph<std::string>::reverse_adjacent_vertex_iterator arbegin(
            adjacent.second);
        lab4::graph<std::string>::reverse_adjacent_vertex_iterator arend(
            adjacent.first);
        CHECK(arbegin == arend);
    }

    CHECK_THROW(graph.incident_edges("missing"), std::out_of_range);
    CHECK_THROW(graph.adjacent_vertices("missing"), std::out_of_range);
}

void test_graph_const_iterators() {
    lab4::graph<std::string> graph;
    graph.add_vertex("a");
    graph.add_vertex("b");
    graph.add_vertex("c");
    graph.add_edge("a", "b");
    graph.add_edge("b", "c");

    const lab4::graph<std::string>& constant = graph;

    CHECK(!constant.empty());
    CHECK(constant.vertex_count() == 3);
    CHECK(constant.edge_count() == 2);
    CHECK(constant.has_vertex("a"));
    CHECK(!constant.has_vertex("z"));
    CHECK(constant.has_edge("a", "b"));
    CHECK(!constant.has_edge("a", "c"));
    CHECK(constant.degree("b") == 2);
    CHECK(constant.edge_degree("a", "b") == 2);
    CHECK_THROW(constant.degree("z"), std::out_of_range);
    CHECK_THROW(constant.edge_degree("a", "c"), std::out_of_range);

    {
        std::vector<std::string> values;
        for (auto it = constant.begin(); it != constant.end(); ++it) {
            values.push_back(*it);
        }
        CHECK(values == std::vector<std::string>({"a", "b", "c"}));

        std::vector<std::string> values2;
        for (auto it = constant.cbegin(); it != constant.cend(); ++it) {
            values2.push_back(*it);
        }
        CHECK(values == values2);

        CHECK(constant.begin()->size() == 1);
    }

    {
        std::vector<std::string> values;
        for (auto it = constant.rbegin(); it != constant.rend(); ++it) {
            values.push_back(*it);
        }
        CHECK(values == std::vector<std::string>({"c", "b", "a"}));

        std::vector<std::string> values2;
        for (auto it = constant.crbegin(); it != constant.crend(); ++it) {
            values2.push_back(*it);
        }
        CHECK(values == values2);
    }

    {
        std::vector<std::pair<std::string, std::string>> edges;
        for (auto it = constant.edges_begin(); it != constant.edges_end();
             ++it) {
            edges.push_back(*it);
        }
        CHECK(edges ==
              std::vector<std::pair<std::string, std::string>>(
                  {{"a", "b"}, {"b", "c"}}));

        std::vector<std::pair<std::string, std::string>> edges2;
        for (auto it = constant.edges_cbegin(); it != constant.edges_cend();
             ++it) {
            edges2.push_back(*it);
        }
        CHECK(edges == edges2);
    }

    {
        std::vector<std::pair<std::string, std::string>> edges;
        for (auto it = constant.edges_rbegin(); it != constant.edges_rend();
             ++it) {
            edges.push_back(*it);
        }
        CHECK(edges ==
              std::vector<std::pair<std::string, std::string>>(
                  {{"b", "c"}, {"a", "b"}}));

        std::vector<std::pair<std::string, std::string>> edges2;
        for (auto it = constant.edges_crbegin(); it != constant.edges_crend();
             ++it) {
            edges2.push_back(*it);
        }
        CHECK(edges == edges2);
    }

    {
        auto range = constant.incident_edges("b");
        std::vector<std::pair<std::string, std::string>> edges;
        for (auto it = range.first; it != range.second; ++it) {
            edges.push_back(*it);
        }
        CHECK(edges ==
              std::vector<std::pair<std::string, std::string>>(
                  {{"a", "b"}, {"b", "c"}}));
        CHECK_THROW(constant.incident_edges("z"), std::out_of_range);

        auto reverse_range = constant.reverse_incident_edges("b");
        std::vector<std::pair<std::string, std::string>> reversed;
        for (auto it = reverse_range.first; it != reverse_range.second; ++it) {
            reversed.push_back(*it);
        }
        CHECK(reversed ==
              std::vector<std::pair<std::string, std::string>>(
                  {{"b", "c"}, {"a", "b"}}));
        CHECK_THROW(constant.reverse_incident_edges("z"), std::out_of_range);
    }

    {
        auto range = constant.adjacent_vertices("b");
        std::vector<std::string> values;
        for (auto it = range.first; it != range.second; ++it) {
            values.push_back(*it);
        }
        CHECK(values == std::vector<std::string>({"a", "c"}));
        CHECK_THROW(constant.adjacent_vertices("z"), std::out_of_range);

        auto reverse_range = constant.reverse_adjacent_vertices("b");
        std::vector<std::string> reversed;
        for (auto it = reverse_range.first; it != reverse_range.second; ++it) {
            reversed.push_back(*it);
        }
        CHECK(reversed == std::vector<std::string>({"c", "a"}));
        CHECK_THROW(constant.reverse_adjacent_vertices("z"),
                    std::out_of_range);
    }

    const lab4::graph<std::string>::const_vertex_iterator vertex =
        constant.begin();
    CHECK(*vertex == "a");
    const lab4::graph<std::string>::const_edge_iterator edge =
        constant.edges_begin();
    CHECK(*edge == std::make_pair(std::string("a"), std::string("b")));
}

void test_graph_erase() {
    {
        lab4::graph<int> graph;
        for (int value = 1; value <= 4; ++value) {
            graph.add_vertex(value);
        }
        graph.add_edge(1, 2);
        graph.add_edge(2, 3);
        graph.add_edge(3, 4);

        auto middle = find_vertex(graph, 2);
        graph.erase_vertex(middle);
        CHECK(graph.vertex_count() == 3);
        CHECK(graph.edge_count() == 1);
        CHECK(!graph.has_vertex(2));
        CHECK(!graph.has_edge(1, 2));
        CHECK(!graph.has_edge(2, 3));
        CHECK(graph.has_edge(3, 4));
        CHECK(graph.degree(3) == 1);

        auto first = graph.begin();
        graph.erase_vertex(first);
        CHECK(graph.vertex_count() == 2);
        CHECK(!graph.has_vertex(1));

        auto last = graph.end();
        --last;
        graph.erase_vertex(last);
        CHECK(graph.vertex_count() == 1);
        CHECK(graph.has_vertex(3));
        CHECK(graph.edge_count() == 0);

        CHECK_THROW(graph.erase_vertex(graph.end()), std::invalid_argument);

        lab4::graph<int> other;
        other.add_vertex(10);
        CHECK_THROW(graph.erase_vertex(other.begin()), std::invalid_argument);

        graph.clear();
        CHECK(graph.empty());
    }

    {
        lab4::graph<int> graph;
        graph.add_vertex(0);
        for (int value = 1; value <= 4; ++value) {
            graph.add_vertex(value);
            graph.add_edge(0, value);
        }

        auto middle = graph.edges_begin();
        ++middle;
        ++middle;
        CHECK(*middle == std::make_pair(0, 3));
        graph.erase_edge(middle);
        CHECK(graph.edge_count() == 3);
        CHECK(!graph.has_edge(0, 3));
        CHECK(graph.degree(0) == 3);
        CHECK(graph.degree(3) == 0);

        graph.erase_edge(graph.edges_begin());
        CHECK(graph.edge_count() == 2);
        CHECK(!graph.has_edge(0, 1));
        CHECK(graph.degree(1) == 0);

        auto last = graph.edges_end();
        --last;
        CHECK(*last == std::make_pair(0, 4));
        graph.erase_edge(last);
        CHECK(graph.edge_count() == 1);
        CHECK(!graph.has_edge(0, 4));
        CHECK(graph.has_edge(0, 2));
        CHECK(graph.degree(0) == 1);

        CHECK_THROW(graph.erase_edge(graph.edges_end()), std::invalid_argument);

        lab4::graph<int> other;
        other.add_vertex(5);
        other.add_vertex(6);
        other.add_edge(5, 6);
        CHECK_THROW(graph.erase_edge(other.edges_begin()),
                    std::invalid_argument);
    }
}

void test_graph_copy_and_assignment() {
    lab4::graph<int> original;
    original.add_vertex(1);
    original.add_vertex(2);
    original.add_vertex(3);
    original.add_edge(1, 2);
    original.add_edge(2, 3);

    lab4::graph<int> copy(original);
    CHECK(copy == original);
    CHECK(copy.vertex_count() == 3);
    CHECK(copy.edge_count() == 2);
    CHECK(copy.has_edge(1, 2));
    CHECK(copy.has_edge(3, 2));

    copy.add_vertex(4);
    CHECK(copy != original);
    CHECK(!original.has_vertex(4));

    copy = original;
    CHECK(copy == original);
    CHECK(!copy.has_vertex(4));

    lab4::graph<int>& self = copy;
    copy = self;
    CHECK(copy == original);

    original.clear();
    CHECK(original.empty());
    CHECK(copy.vertex_count() == 3);
    CHECK(copy.edge_count() == 2);

    lab4::graph<int> empty;
    lab4::graph<int> empty_copy(empty);
    CHECK(empty_copy.empty());
    empty_copy = empty;
    CHECK(empty_copy.empty());

    lab4::graph<int> assigned;
    assigned = copy;
    CHECK(assigned == copy);
    lab4::graph<int>& assigned_self = assigned;
    assigned = assigned_self;
    CHECK(assigned == copy);
}

void test_graph_comparison() {
    lab4::graph<int> first;
    first.add_vertex(1);
    first.add_vertex(2);
    first.add_vertex(3);
    first.add_edge(1, 2);
    first.add_edge(2, 3);

    lab4::graph<int> same;
    same.add_vertex(3);
    same.add_vertex(2);
    same.add_vertex(1);
    same.add_edge(3, 2);
    same.add_edge(2, 1);

    CHECK(first == same);
    CHECK(!(first != same));
    CHECK(!(first < same));
    CHECK(!(first > same));
    CHECK(first <= same);
    CHECK(first >= same);

    lab4::graph<int> fewer_edges;
    fewer_edges.add_vertex(1);
    fewer_edges.add_vertex(2);
    fewer_edges.add_vertex(3);
    fewer_edges.add_edge(1, 2);

    CHECK(first != fewer_edges);
    CHECK(fewer_edges < first);
    CHECK(fewer_edges <= first);
    CHECK(first > fewer_edges);
    CHECK(first >= fewer_edges);
    CHECK(!(first < fewer_edges));
    CHECK(!(fewer_edges > first));

    lab4::graph<int> fewer_vertices;
    fewer_vertices.add_vertex(1);
    fewer_vertices.add_vertex(2);
    fewer_vertices.add_edge(1, 2);

    CHECK(fewer_vertices < first);
    CHECK(first > fewer_vertices);
    CHECK(fewer_vertices != first);

    lab4::graph<int> other_edge;
    other_edge.add_vertex(1);
    other_edge.add_vertex(2);
    other_edge.add_vertex(3);
    other_edge.add_edge(1, 3);

    CHECK(first < other_edge);
    CHECK(other_edge > first);
    CHECK(first <= other_edge);
    CHECK(other_edge >= first);
    CHECK(!(other_edge < first));
    CHECK(!(first > other_edge));

    const lab4::graph<int>& constant = first;
    CHECK(constant == same);
    CHECK(constant < other_edge);
    CHECK(constant <= same);
    CHECK(constant >= same);

    lab4::graph<int> empty1;
    lab4::graph<int> empty2;
    CHECK(empty1 == empty2);
    CHECK(!(empty1 < empty2));
    CHECK(empty1 <= empty2);
    CHECK(empty1 >= empty2);
    CHECK(empty1 < first);

    lab4::graph<int> two_vertices_a;
    two_vertices_a.add_vertex(1);
    two_vertices_a.add_vertex(2);

    lab4::graph<int> two_vertices_b;
    two_vertices_b.add_vertex(1);
    two_vertices_b.add_vertex(3);

    CHECK(two_vertices_a != two_vertices_b);
    CHECK(!(two_vertices_a == two_vertices_b));

    lab4::graph<int> different_edges;
    different_edges.add_vertex(1);
    different_edges.add_vertex(2);
    different_edges.add_vertex(3);
    different_edges.add_edge(1, 3);
    different_edges.add_edge(2, 3);

    CHECK(first != different_edges);
    CHECK(!(first == different_edges));
}

void test_graph_copy_exception_safety() {
    using graph_type = lab4::graph<ThrowingValue>;

    graph_type source;
    source.add_vertex(ThrowingValue(1));
    source.add_vertex(ThrowingValue(2));

    ThrowingValue::copies_before_throw = 1;
    CHECK_THROW(graph_type copy(source), std::runtime_error);
    ThrowingValue::copies_before_throw = -1;

    graph_type copy(source);
    CHECK(copy.vertex_count() == 2);
    CHECK(copy.has_vertex(ThrowingValue(1)));
    CHECK(copy.has_vertex(ThrowingValue(2)));
}

void test_graph_output() {
    {
        lab4::graph<int> graph;
        graph.add_vertex(1);
        graph.add_vertex(2);
        graph.add_vertex(3);
        graph.add_edge(1, 2);
        graph.add_edge(2, 3);

        std::ostringstream out;
        out << graph;
        CHECK(out.str() ==
              "graph{vertices:[1, 2, 3], edges:[(1, 2), (2, 3)]}");
    }

    {
        lab4::graph<int> graph;
        std::ostringstream out;
        out << graph;
        CHECK(out.str() == "graph{vertices:[], edges:[]}");
    }

    {
        lab4::graph<std::string> graph;
        graph.add_vertex("x");
        graph.add_vertex("y");
        graph.add_edge("y", "x");
        std::ostringstream out;
        out << graph;
        CHECK(out.str() == "graph{vertices:[x, y], edges:[(y, x)]}");
    }
}

} // namespace

int main() {
    test_smoothsort();
    test_pancake_sort();
    test_graph_basics();
    test_graph_iterators();
    test_graph_const_iterators();
    test_graph_erase();
    test_graph_copy_and_assignment();
    test_graph_comparison();
    test_graph_copy_exception_safety();
    test_graph_output();

    if (failures == 0) {
        std::cout << "All " << checks << " checks passed.\n";
        return 0;
    }
    std::cerr << failures << " of " << checks << " checks failed.\n";
    return 1;
}
