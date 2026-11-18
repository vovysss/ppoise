#pragma once

#include <algorithm>
#include <cctype>
#include <ctime>
#include <string>

namespace mp {
namespace util {

inline std::string toLower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return value;
}

inline bool containsIgnoreCase(const std::string& haystack, const std::string& needle) {
    return toLower(haystack).find(toLower(needle)) != std::string::npos;
}

inline bool containsWithCase(const std::string& haystack, const std::string& needle, bool caseSensitive) {
    if (caseSensitive) {
        return haystack.find(needle) != std::string::npos;
    }
    return containsIgnoreCase(haystack, needle);
}

inline std::time_t now() {
    return std::time(nullptr);
}

inline std::string formatDuration(int seconds) {
    if (seconds < 0) {
        seconds = 0;
    }
    const int hours = seconds / 3600;
    const int minutes = (seconds % 3600) / 60;
    const int remaining = seconds % 60;

    auto two = [](int value) {
        return (value < 10 ? "0" : "") + std::to_string(value);
    };

    if (hours > 0) {
        return std::to_string(hours) + ":" + two(minutes) + ":" + two(remaining);
    }
    return std::to_string(minutes) + ":" + two(remaining);
}

inline std::string simpleHash(const std::string& password) {
    unsigned long long hash = 1469598103934665603ULL;
    for (unsigned char c : password) {
        hash ^= c;
        hash *= 1099511628211ULL;
    }
    return std::to_string(hash);
}

inline std::string extensionOf(const std::string& path) {
    const std::size_t dot = path.find_last_of('.');
    if (dot == std::string::npos || dot + 1 >= path.size()) {
        return "";
    }
    return toLower(path.substr(dot + 1));
}

} // namespace util
} // namespace mp
