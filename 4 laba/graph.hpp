#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <ostream>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace lab4 {

// Неориентированный граф на модифицированной структуре Вирта:
// список вершин, у каждой вершины — двунаправленный список инцидентностей,
// плюс общий двунаправленный список рёбер. Внутреннее представление скрыто.
template <class T>
class graph {
public:
    using value_type = T;
    using edge_type = std::pair<T, T>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

private:
    struct incidence;
    struct edge_node;

    struct vertex_node {
        T value;
        vertex_node* prev = nullptr;
        vertex_node* next = nullptr;
        incidence* inc_head = nullptr;
        incidence* inc_tail = nullptr;
        size_type degree = 0;
    };

    struct edge_node {
        vertex_node* u = nullptr;
        vertex_node* v = nullptr;
        edge_node* prev = nullptr;
        edge_node* next = nullptr;
        incidence* inc_u = nullptr;
        incidence* inc_v = nullptr;
    };

    struct incidence {
        vertex_node* owner = nullptr;
        edge_node* edge = nullptr;
        vertex_node* neighbor = nullptr;
        incidence* prev = nullptr;
        incidence* next = nullptr;
    };

public:
    template <bool IsConst>
    class basic_vertex_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using reference = std::conditional_t<IsConst, const T&, T&>;
        using pointer = std::conditional_t<IsConst, const T*, T*>;

    private:
        using graph_pointer = std::conditional_t<IsConst, const graph*, graph*>;
        using node_pointer =
            std::conditional_t<IsConst, const vertex_node*, vertex_node*>;

    public:
        basic_vertex_iterator() = default;

        template <bool C = IsConst, typename = std::enable_if_t<C>>
        basic_vertex_iterator(const basic_vertex_iterator<false>& other)
            : graph_(other.graph_), node_(other.node_) {}

        reference operator*() const { return node_->value; }
        pointer operator->() const { return &node_->value; }

        basic_vertex_iterator& operator++() {
            node_ = node_->next;
            return *this;
        }

        basic_vertex_iterator operator++(int) {
            basic_vertex_iterator copy(*this);
            ++(*this);
            return copy;
        }

        basic_vertex_iterator& operator--() {
            node_ = node_ ? node_->prev : graph_->vertex_tail_;
            return *this;
        }

        basic_vertex_iterator operator--(int) {
            basic_vertex_iterator copy(*this);
            --(*this);
            return copy;
        }

        template <bool C>
        bool operator==(const basic_vertex_iterator<C>& other) const {
            return graph_ == other.graph_ && node_ == other.node_;
        }

        template <bool C>
        bool operator!=(const basic_vertex_iterator<C>& other) const {
            return !(*this == other);
        }

    private:
        friend class graph;
        template <bool>
        friend class basic_vertex_iterator;

        basic_vertex_iterator(graph_pointer owner, node_pointer node)
            : graph_(owner), node_(node) {}

        graph_pointer graph_ = nullptr;
        node_pointer node_ = nullptr;
    };

    template <bool IsConst>
    class basic_edge_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = edge_type;
        using difference_type = std::ptrdiff_t;
        using reference = edge_type;
        using pointer = void;

    private:
        using graph_pointer = std::conditional_t<IsConst, const graph*, graph*>;
        using node_pointer =
            std::conditional_t<IsConst, const edge_node*, edge_node*>;

    public:
        basic_edge_iterator() = default;

        template <bool C = IsConst, typename = std::enable_if_t<C>>
        basic_edge_iterator(const basic_edge_iterator<false>& other)
            : graph_(other.graph_), node_(other.node_) {}

        reference operator*() const {
            return edge_type{node_->u->value, node_->v->value};
        }

        basic_edge_iterator& operator++() {
            node_ = node_->next;
            return *this;
        }

        basic_edge_iterator operator++(int) {
            basic_edge_iterator copy(*this);
            ++(*this);
            return copy;
        }

        basic_edge_iterator& operator--() {
            node_ = node_ ? node_->prev : graph_->edge_tail_;
            return *this;
        }

        basic_edge_iterator operator--(int) {
            basic_edge_iterator copy(*this);
            --(*this);
            return copy;
        }

        template <bool C>
        bool operator==(const basic_edge_iterator<C>& other) const {
            return graph_ == other.graph_ && node_ == other.node_;
        }

        template <bool C>
        bool operator!=(const basic_edge_iterator<C>& other) const {
            return !(*this == other);
        }

    private:
        friend class graph;
        template <bool>
        friend class basic_edge_iterator;

        basic_edge_iterator(graph_pointer owner, node_pointer node)
            : graph_(owner), node_(node) {}

        graph_pointer graph_ = nullptr;
        node_pointer node_ = nullptr;
    };

    template <bool IsConst>
    class basic_incident_edge_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = edge_type;
        using difference_type = std::ptrdiff_t;
        using reference = edge_type;
        using pointer = void;

    private:
        using vertex_pointer =
            std::conditional_t<IsConst, const vertex_node*, vertex_node*>;
        using incidence_pointer =
            std::conditional_t<IsConst, const incidence*, incidence*>;

    public:
        basic_incident_edge_iterator() = default;

        template <bool C = IsConst, typename = std::enable_if_t<C>>
        basic_incident_edge_iterator(
            const basic_incident_edge_iterator<false>& other)
            : owner_(other.owner_), node_(other.node_) {}

        reference operator*() const {
            return edge_type{node_->edge->u->value, node_->edge->v->value};
        }

        basic_incident_edge_iterator& operator++() {
            node_ = node_ ? node_->next : nullptr;
            return *this;
        }

        basic_incident_edge_iterator operator++(int) {
            basic_incident_edge_iterator copy(*this);
            ++(*this);
            return copy;
        }

        basic_incident_edge_iterator& operator--() {
            node_ = node_ ? node_->prev : owner_->inc_tail;
            return *this;
        }

        basic_incident_edge_iterator operator--(int) {
            basic_incident_edge_iterator copy(*this);
            --(*this);
            return copy;
        }

        template <bool C>
        bool operator==(const basic_incident_edge_iterator<C>& other) const {
            return owner_ == other.owner_ && node_ == other.node_;
        }

        template <bool C>
        bool operator!=(const basic_incident_edge_iterator<C>& other) const {
            return !(*this == other);
        }

    private:
        friend class graph;
        template <bool>
        friend class basic_incident_edge_iterator;

        basic_incident_edge_iterator(vertex_pointer owner,
                                     incidence_pointer node)
            : owner_(owner), node_(node) {}

        vertex_pointer owner_ = nullptr;
        incidence_pointer node_ = nullptr;
    };

    template <bool IsConst>
    class basic_adjacent_vertex_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using reference = std::conditional_t<IsConst, const T&, T&>;
        using pointer = std::conditional_t<IsConst, const T*, T*>;

    private:
        using vertex_pointer =
            std::conditional_t<IsConst, const vertex_node*, vertex_node*>;
        using incidence_pointer =
            std::conditional_t<IsConst, const incidence*, incidence*>;

    public:
        basic_adjacent_vertex_iterator() = default;

        template <bool C = IsConst, typename = std::enable_if_t<C>>
        basic_adjacent_vertex_iterator(
            const basic_adjacent_vertex_iterator<false>& other)
            : owner_(other.owner_), node_(other.node_) {}

        reference operator*() const { return node_->neighbor->value; }
        pointer operator->() const { return &node_->neighbor->value; }

        basic_adjacent_vertex_iterator& operator++() {
            node_ = node_ ? node_->next : nullptr;
            return *this;
        }

        basic_adjacent_vertex_iterator operator++(int) {
            basic_adjacent_vertex_iterator copy(*this);
            ++(*this);
            return copy;
        }

        basic_adjacent_vertex_iterator& operator--() {
            node_ = node_ ? node_->prev : owner_->inc_tail;
            return *this;
        }

        basic_adjacent_vertex_iterator operator--(int) {
            basic_adjacent_vertex_iterator copy(*this);
            --(*this);
            return copy;
        }

        template <bool C>
        bool operator==(const basic_adjacent_vertex_iterator<C>& other) const {
            return owner_ == other.owner_ && node_ == other.node_;
        }

        template <bool C>
        bool operator!=(const basic_adjacent_vertex_iterator<C>& other) const {
            return !(*this == other);
        }

    private:
        friend class graph;
        template <bool>
        friend class basic_adjacent_vertex_iterator;

        basic_adjacent_vertex_iterator(vertex_pointer owner,
                                       incidence_pointer node)
            : owner_(owner), node_(node) {}

        vertex_pointer owner_ = nullptr;
        incidence_pointer node_ = nullptr;
    };

    using vertex_iterator = basic_vertex_iterator<false>;
    using const_vertex_iterator = basic_vertex_iterator<true>;
    using reverse_vertex_iterator = std::reverse_iterator<vertex_iterator>;
    using const_reverse_vertex_iterator =
        std::reverse_iterator<const_vertex_iterator>;

    using edge_iterator = basic_edge_iterator<false>;
    using const_edge_iterator = basic_edge_iterator<true>;
    using reverse_edge_iterator = std::reverse_iterator<edge_iterator>;
    using const_reverse_edge_iterator =
        std::reverse_iterator<const_edge_iterator>;

    using incident_edge_iterator = basic_incident_edge_iterator<false>;
    using const_incident_edge_iterator = basic_incident_edge_iterator<true>;
    using reverse_incident_edge_iterator =
        std::reverse_iterator<incident_edge_iterator>;
    using const_reverse_incident_edge_iterator =
        std::reverse_iterator<const_incident_edge_iterator>;

    using adjacent_vertex_iterator = basic_adjacent_vertex_iterator<false>;
    using const_adjacent_vertex_iterator =
        basic_adjacent_vertex_iterator<true>;
    using reverse_adjacent_vertex_iterator =
        std::reverse_iterator<adjacent_vertex_iterator>;
    using const_reverse_adjacent_vertex_iterator =
        std::reverse_iterator<const_adjacent_vertex_iterator>;

    graph() = default;

    graph(const graph& other) {
        try {
            for (auto it = other.begin(); it != other.end(); ++it) {
                add_vertex(*it);
            }
            for (auto it = other.edges_begin(); it != other.edges_end(); ++it) {
                add_edge((*it).first, (*it).second);
            }
        } catch (...) {
            clear();
            throw;
        }
    }

    ~graph() { clear(); }

    graph& operator=(const graph& other) {
        if (this != &other) {
            graph copy(other);
            swap(copy);
        }
        return *this;
    }

    bool empty() const noexcept { return vertex_count_ == 0; }
    size_type vertex_count() const noexcept { return vertex_count_; }
    size_type edge_count() const noexcept { return edge_count_; }

    void clear() noexcept {
        while (vertex_head_ != nullptr) {
            erase_vertex_node(vertex_head_);
        }
        vertex_count_ = 0;
        edge_count_ = 0;
    }

    bool has_vertex(const value_type& value) const {
        return find_vertex_node(value) != nullptr;
    }

    bool has_edge(const value_type& first, const value_type& second) const {
        return find_edge_node(first, second) != nullptr;
    }

    size_type degree(const value_type& value) const {
        const vertex_node* node = find_vertex_node(value);
        if (node == nullptr) {
            throw std::out_of_range("graph: vertex not found");
        }
        return node->degree;
    }

    size_type edge_degree(const value_type& first,
                          const value_type& second) const {
        const edge_node* edge = find_edge_node(first, second);
        if (edge == nullptr) {
            throw std::out_of_range("graph: edge not found");
        }
        return (edge->inc_u != nullptr ? 1u : 0u) +
               (edge->inc_v != nullptr ? 1u : 0u);
    }

    size_type edge_degree(const edge_type& edge) const {
        return edge_degree(edge.first, edge.second);
    }

    vertex_iterator add_vertex(const value_type& value) {
        if (find_vertex_node(value) != nullptr) {
            throw std::invalid_argument("graph: vertex already exists");
        }

        vertex_node* node = new vertex_node{value};
        node->prev = vertex_tail_;
        if (vertex_tail_ != nullptr) {
            vertex_tail_->next = node;
        } else {
            vertex_head_ = node;
        }
        vertex_tail_ = node;
        ++vertex_count_;
        return vertex_iterator(this, node);
    }

    edge_iterator add_edge(const value_type& first, const value_type& second) {
        if (first == second) {
            throw std::invalid_argument("graph: self-loops are not allowed");
        }

        vertex_node* u = find_vertex_node(first);
        vertex_node* v = find_vertex_node(second);
        if (u == nullptr || v == nullptr) {
            throw std::out_of_range("graph: vertex not found");
        }
        if (find_edge_node(first, second) != nullptr) {
            throw std::invalid_argument("graph: edge already exists");
        }

        edge_node* edge = new edge_node;
        edge->u = u;
        edge->v = v;

        incidence* inc_u = nullptr;
        incidence* inc_v = nullptr;
        try {
            inc_u = new incidence{u, edge, v};
            inc_v = new incidence{v, edge, u};
        } catch (...) {
            delete inc_u;
            delete edge;
            throw;
        }

        edge->inc_u = inc_u;
        edge->inc_v = inc_v;
        append_incidence(u, inc_u);
        append_incidence(v, inc_v);
        append_edge(edge);
        ++edge_count_;
        return edge_iterator(this, edge);
    }

    // Удаление вершины по итератору вместе со всеми инцидентными рёбрами.
    void erase_vertex(vertex_iterator position) {
        if (position.graph_ != this || position.node_ == nullptr) {
            throw std::invalid_argument("graph: invalid vertex iterator");
        }
        erase_vertex_node(position.node_);
    }

    // Удаление ребра по итератору.
    void erase_edge(edge_iterator position) {
        if (position.graph_ != this || position.node_ == nullptr) {
            throw std::invalid_argument("graph: invalid edge iterator");
        }
        erase_edge_node(position.node_);
    }

    vertex_iterator begin() noexcept {
        return vertex_iterator(this, vertex_head_);
    }
    vertex_iterator end() noexcept { return vertex_iterator(this, nullptr); }
    const_vertex_iterator begin() const noexcept {
        return const_vertex_iterator(this, vertex_head_);
    }
    const_vertex_iterator end() const noexcept {
        return const_vertex_iterator(this, nullptr);
    }
    const_vertex_iterator cbegin() const noexcept { return begin(); }
    const_vertex_iterator cend() const noexcept { return end(); }

    reverse_vertex_iterator rbegin() noexcept {
        return reverse_vertex_iterator(end());
    }
    reverse_vertex_iterator rend() noexcept {
        return reverse_vertex_iterator(begin());
    }
    const_reverse_vertex_iterator rbegin() const noexcept {
        return const_reverse_vertex_iterator(end());
    }
    const_reverse_vertex_iterator rend() const noexcept {
        return const_reverse_vertex_iterator(begin());
    }
    const_reverse_vertex_iterator crbegin() const noexcept { return rbegin(); }
    const_reverse_vertex_iterator crend() const noexcept { return rend(); }

    edge_iterator edges_begin() noexcept {
        return edge_iterator(this, edge_head_);
    }
    edge_iterator edges_end() noexcept {
        return edge_iterator(this, nullptr);
    }
    const_edge_iterator edges_begin() const noexcept {
        return const_edge_iterator(this, edge_head_);
    }
    const_edge_iterator edges_end() const noexcept {
        return const_edge_iterator(this, nullptr);
    }
    const_edge_iterator edges_cbegin() const noexcept {
        return edges_begin();
    }
    const_edge_iterator edges_cend() const noexcept { return edges_end(); }

    reverse_edge_iterator edges_rbegin() noexcept {
        return reverse_edge_iterator(edges_end());
    }
    reverse_edge_iterator edges_rend() noexcept {
        return reverse_edge_iterator(edges_begin());
    }
    const_reverse_edge_iterator edges_rbegin() const noexcept {
        return const_reverse_edge_iterator(edges_end());
    }
    const_reverse_edge_iterator edges_rend() const noexcept {
        return const_reverse_edge_iterator(edges_begin());
    }
    const_reverse_edge_iterator edges_crbegin() const noexcept {
        return edges_rbegin();
    }
    const_reverse_edge_iterator edges_crend() const noexcept {
        return edges_rend();
    }

    std::pair<incident_edge_iterator, incident_edge_iterator>
    incident_edges(const value_type& value) {
        vertex_node* node = find_vertex_node(value);
        if (node == nullptr) {
            throw std::out_of_range("graph: vertex not found");
        }
        return {incident_edge_iterator(node, node->inc_head),
                incident_edge_iterator(node, nullptr)};
    }

    std::pair<const_incident_edge_iterator, const_incident_edge_iterator>
    incident_edges(const value_type& value) const {
        const vertex_node* node = find_vertex_node(value);
        if (node == nullptr) {
            throw std::out_of_range("graph: vertex not found");
        }
        return {const_incident_edge_iterator(node, node->inc_head),
                const_incident_edge_iterator(node, nullptr)};
    }

    std::pair<adjacent_vertex_iterator, adjacent_vertex_iterator>
    adjacent_vertices(const value_type& value) {
        vertex_node* node = find_vertex_node(value);
        if (node == nullptr) {
            throw std::out_of_range("graph: vertex not found");
        }
        return {adjacent_vertex_iterator(node, node->inc_head),
                adjacent_vertex_iterator(node, nullptr)};
    }

    std::pair<const_adjacent_vertex_iterator,
              const_adjacent_vertex_iterator>
    adjacent_vertices(const value_type& value) const {
        const vertex_node* node = find_vertex_node(value);
        if (node == nullptr) {
            throw std::out_of_range("graph: vertex not found");
        }
        return {const_adjacent_vertex_iterator(node, node->inc_head),
                const_adjacent_vertex_iterator(node, nullptr)};
    }

    std::pair<reverse_incident_edge_iterator, reverse_incident_edge_iterator>
    reverse_incident_edges(const value_type& value) {
        const auto range = incident_edges(value);
        return {reverse_incident_edge_iterator(range.second),
                reverse_incident_edge_iterator(range.first)};
    }

    std::pair<const_reverse_incident_edge_iterator,
              const_reverse_incident_edge_iterator>
    reverse_incident_edges(const value_type& value) const {
        const auto range = incident_edges(value);
        return {const_reverse_incident_edge_iterator(range.second),
                const_reverse_incident_edge_iterator(range.first)};
    }

    std::pair<reverse_adjacent_vertex_iterator,
              reverse_adjacent_vertex_iterator>
    reverse_adjacent_vertices(const value_type& value) {
        const auto range = adjacent_vertices(value);
        return {reverse_adjacent_vertex_iterator(range.second),
                reverse_adjacent_vertex_iterator(range.first)};
    }

    std::pair<const_reverse_adjacent_vertex_iterator,
              const_reverse_adjacent_vertex_iterator>
    reverse_adjacent_vertices(const value_type& value) const {
        const auto range = adjacent_vertices(value);
        return {const_reverse_adjacent_vertex_iterator(range.second),
                const_reverse_adjacent_vertex_iterator(range.first)};
    }

    bool operator==(const graph& other) const {
        if (vertex_count_ != other.vertex_count_ ||
            edge_count_ != other.edge_count_) {
            return false;
        }

        for (const_vertex_iterator it = begin(); it != end(); ++it) {
            if (!other.has_vertex(*it)) {
                return false;
            }
        }

        for (const_edge_iterator it = edges_begin(); it != edges_end(); ++it) {
            const edge_type edge = *it;
            if (!other.has_edge(edge.first, edge.second)) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const graph& other) const { return !(*this == other); }

    bool operator<(const graph& other) const {
        const std::vector<T> vertices = vertex_values();
        const std::vector<T> other_vertices = other.vertex_values();
        if (vertices != other_vertices) {
            return vertices < other_vertices;
        }
        return canonical_edges() < other.canonical_edges();
    }

    bool operator<=(const graph& other) const { return !(other < *this); }
    bool operator>(const graph& other) const { return other < *this; }
    bool operator>=(const graph& other) const { return !(*this < other); }

private:
    const vertex_node* find_vertex_node(const value_type& value) const {
        for (const vertex_node* node = vertex_head_; node != nullptr;
             node = node->next) {
            if (node->value == value) {
                return node;
            }
        }
        return nullptr;
    }

    vertex_node* find_vertex_node(const value_type& value) {
        return const_cast<vertex_node*>(
            static_cast<const graph*>(this)->find_vertex_node(value));
    }

    const edge_node* find_edge_node(const value_type& first,
                                    const value_type& second) const {
        const vertex_node* u = find_vertex_node(first);
        const vertex_node* v = find_vertex_node(second);
        if (u == nullptr || v == nullptr) {
            return nullptr;
        }

        for (const incidence* inc = u->inc_head; inc != nullptr;
             inc = inc->next) {
            if (inc->neighbor == v) {
                return inc->edge;
            }
        }
        return nullptr;
    }

    edge_node* find_edge_node(const value_type& first,
                              const value_type& second) {
        return const_cast<edge_node*>(
            static_cast<const graph*>(this)->find_edge_node(first, second));
    }

    void append_incidence(vertex_node* owner, incidence* inc) noexcept {
        inc->prev = owner->inc_tail;
        inc->next = nullptr;
        if (owner->inc_tail != nullptr) {
            owner->inc_tail->next = inc;
        } else {
            owner->inc_head = inc;
        }
        owner->inc_tail = inc;
        ++owner->degree;
    }

    void append_edge(edge_node* edge) noexcept {
        edge->prev = edge_tail_;
        edge->next = nullptr;
        if (edge_tail_ != nullptr) {
            edge_tail_->next = edge;
        } else {
            edge_head_ = edge;
        }
        edge_tail_ = edge;
    }

    void unlink_incidence(incidence* inc) noexcept {
        vertex_node* owner = inc->owner;
        if (inc->prev != nullptr) {
            inc->prev->next = inc->next;
        } else {
            owner->inc_head = inc->next;
        }
        if (inc->next != nullptr) {
            inc->next->prev = inc->prev;
        } else {
            owner->inc_tail = inc->prev;
        }
        --owner->degree;
        delete inc;
    }

    void erase_edge_node(edge_node* edge) noexcept {
        unlink_incidence(edge->inc_u);
        unlink_incidence(edge->inc_v);

        if (edge->prev != nullptr) {
            edge->prev->next = edge->next;
        } else {
            edge_head_ = edge->next;
        }
        if (edge->next != nullptr) {
            edge->next->prev = edge->prev;
        } else {
            edge_tail_ = edge->prev;
        }

        delete edge;
        --edge_count_;
    }

    void erase_vertex_node(vertex_node* node) noexcept {
        while (node->inc_head != nullptr) {
            erase_edge_node(node->inc_head->edge);
        }

        if (node->prev != nullptr) {
            node->prev->next = node->next;
        } else {
            vertex_head_ = node->next;
        }
        if (node->next != nullptr) {
            node->next->prev = node->prev;
        } else {
            vertex_tail_ = node->prev;
        }

        delete node;
        --vertex_count_;
    }

    std::vector<T> vertex_values() const {
        std::vector<T> values;
        values.reserve(vertex_count_);
        for (const_vertex_iterator it = begin(); it != end(); ++it) {
            values.push_back(*it);
        }
        std::sort(values.begin(), values.end());
        return values;
    }

    std::vector<edge_type> canonical_edges() const {
        std::vector<edge_type> edges;
        edges.reserve(edge_count_);
        for (const_edge_iterator it = edges_begin(); it != edges_end(); ++it) {
            edge_type edge = *it;
            if (edge.second < edge.first) {
                std::swap(edge.first, edge.second);
            }
            edges.push_back(std::move(edge));
        }
        std::sort(edges.begin(), edges.end());
        return edges;
    }

    void swap(graph& other) noexcept {
        std::swap(vertex_head_, other.vertex_head_);
        std::swap(vertex_tail_, other.vertex_tail_);
        std::swap(edge_head_, other.edge_head_);
        std::swap(edge_tail_, other.edge_tail_);
        std::swap(vertex_count_, other.vertex_count_);
        std::swap(edge_count_, other.edge_count_);
    }

    vertex_node* vertex_head_ = nullptr;
    vertex_node* vertex_tail_ = nullptr;
    edge_node* edge_head_ = nullptr;
    edge_node* edge_tail_ = nullptr;
    size_type vertex_count_ = 0;
    size_type edge_count_ = 0;
};

// Текстовое представление строится только через итераторы и std::for_each.
template <class T>
std::ostream& operator<<(std::ostream& out, const graph<T>& value) {
    out << "graph{vertices:[";
    bool first = true;
    std::for_each(value.begin(), value.end(), [&](const T& vertex) {
        if (!first) {
            out << ", ";
        }
        out << vertex;
        first = false;
    });

    out << "], edges:[";
    first = true;
    std::for_each(value.edges_begin(), value.edges_end(),
                  [&](const std::pair<T, T>& edge) {
                      if (!first) {
                          out << ", ";
                      }
                      out << '(' << edge.first << ", " << edge.second << ')';
                      first = false;
                  });
    out << "]}";
    return out;
}

} // namespace lab4
