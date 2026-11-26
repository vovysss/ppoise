#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <utility>
#include <vector>

namespace lab4 {

namespace detail {

// Числа Леонардо: L[0] = L[1] = 1, L[k] = L[k-1] + L[k-2] + 1.
inline std::vector<std::size_t> leonardo_numbers(std::size_t n) {
    std::vector<std::size_t> lp{1, 1};
    while (lp.back() < n + 2) {
        const std::size_t size = lp.size();
        lp.push_back(lp[size - 1] + lp[size - 2] + 1);
    }
    return lp;
}

inline std::size_t trailing_zeros(std::size_t value) {
    std::size_t count = 0;
    while ((value & 1u) == 0u) {
        value >>= 1u;
        ++count;
    }
    return count;
}

// Восстанавливает свойство кучи внутри одного дерева Леонардо.
template <class RandomIt, class Compare>
void sift(RandomIt first, std::size_t order, std::size_t root,
          const std::vector<std::size_t>& lp, Compare comp) {
    using value_type = typename std::iterator_traits<RandomIt>::value_type;

    value_type value = *(first + static_cast<std::ptrdiff_t>(root));
    while (order > 1) {
        const std::size_t right = root - 1;
        const std::size_t left = root - 1 - lp[order - 2];

        if (!comp(value, *(first + static_cast<std::ptrdiff_t>(left))) &&
            !comp(value, *(first + static_cast<std::ptrdiff_t>(right)))) {
            break;
        }

        if (!comp(*(first + static_cast<std::ptrdiff_t>(left)),
                  *(first + static_cast<std::ptrdiff_t>(right)))) {
            *(first + static_cast<std::ptrdiff_t>(root)) =
                std::move(*(first + static_cast<std::ptrdiff_t>(left)));
            root = left;
            --order;
        } else {
            *(first + static_cast<std::ptrdiff_t>(root)) =
                std::move(*(first + static_cast<std::ptrdiff_t>(right)));
            root = right;
            order -= 2;
        }
    }
    *(first + static_cast<std::ptrdiff_t>(root)) = std::move(value);
}

// Восстанавливает порядок между корнями соседних деревьев.
template <class RandomIt, class Compare>
void trinkle(RandomIt first, std::size_t p, std::size_t order,
             std::size_t root, bool trusty,
             const std::vector<std::size_t>& lp, Compare comp) {
    using value_type = typename std::iterator_traits<RandomIt>::value_type;

    value_type value = *(first + static_cast<std::ptrdiff_t>(root));
    while (p != 1) {
        const std::size_t stepson = root - lp[order];
        if (!comp(value, *(first + static_cast<std::ptrdiff_t>(stepson)))) {
            break;
        }

        if (!trusty && order > 1) {
            const std::size_t right = root - 1;
            const std::size_t left = root - 1 - lp[order - 2];
            if (!comp(*(first + static_cast<std::ptrdiff_t>(right)),
                      *(first + static_cast<std::ptrdiff_t>(stepson))) ||
                !comp(*(first + static_cast<std::ptrdiff_t>(left)),
                      *(first + static_cast<std::ptrdiff_t>(stepson)))) {
                break;
            }
        }

        *(first + static_cast<std::ptrdiff_t>(root)) =
            std::move(*(first + static_cast<std::ptrdiff_t>(stepson)));
        root = stepson;

        const std::size_t shift = trailing_zeros(p & ~std::size_t{1});
        p >>= shift;
        order += shift;
        trusty = false;
    }

    if (!trusty) {
        *(first + static_cast<std::ptrdiff_t>(root)) = std::move(value);
        sift(first, order, root, lp, comp);
    }
}

} // namespace detail

// Smoothsort (Дейкстра), сортировка по возрастанию согласно comp.
template <class RandomIt, class Compare>
void smoothsort(RandomIt first, RandomIt last, Compare comp) {
    const std::size_t n =
        static_cast<std::size_t>(std::distance(first, last));
    if (n < 2) {
        return;
    }

    const std::size_t hi = n - 1;
    const std::vector<std::size_t> lp = detail::leonardo_numbers(n);

    std::size_t p = 1;
    std::size_t order = 1;
    std::size_t head = 0;

    // Построение леонардовой кучи слева направо.
    while (head < hi) {
        if ((p & 3u) == 3u) {
            detail::sift(first, order, head, lp, comp);
            p >>= 2u;
            order += 2;
        } else {
            if (order > 0 && lp[order - 1] >= hi - head) {
                detail::trinkle(first, p, order, head, false, lp, comp);
            } else {
                detail::sift(first, order, head, lp, comp);
            }

            if (order == 1) {
                p <<= 1u;
                order = 0;
            } else {
                p <<= (order - 1);
                order = 1;
            }
        }
        p |= 1u;
        ++head;
    }

    detail::trinkle(first, p, order, head, false, lp, comp);

    // Извлечение максимумов справа налево.
    while (order != 1 || p != 1) {
        if (order <= 1) {
            const std::size_t shift =
                detail::trailing_zeros(p & ~std::size_t{1});
            p >>= shift;
            order += shift;
        } else {
            p <<= 2u;
            p ^= 7u;
            order -= 2;

            detail::trinkle(first, p >> 1u, order + 1,
                            head - lp[order] - 1, true, lp, comp);
            detail::trinkle(first, p, order, head - 1, true, lp, comp);
        }
        --head;
    }
}

template <class RandomIt>
void smoothsort(RandomIt first, RandomIt last) {
    using value_type = typename std::iterator_traits<RandomIt>::value_type;
    smoothsort(first, last, std::less<value_type>());
}

// Simple pancake sort: максимум префикса двумя переворотами отправляется в конец.
template <class RandomIt, class Compare>
void pancake_sort(RandomIt first, RandomIt last, Compare comp) {
    using difference_type =
        typename std::iterator_traits<RandomIt>::difference_type;

    for (difference_type size = last - first; size > 1; --size) {
        RandomIt stop = first + size;
        RandomIt largest = first;

        for (RandomIt it = first + 1; it != stop; ++it) {
            if (comp(*largest, *it)) {
                largest = it;
            }
        }

        if (largest + 1 != stop) {
            std::reverse(first, largest + 1);
            std::reverse(first, stop);
        }
    }
}

template <class RandomIt>
void pancake_sort(RandomIt first, RandomIt last) {
    using value_type = typename std::iterator_traits<RandomIt>::value_type;
    pancake_sort(first, last, std::less<value_type>());
}

} // namespace lab4
