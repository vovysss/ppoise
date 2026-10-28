#include "rectangle.h"
#include <algorithm>

void Rectangle::normalize() {
    if (x1 > x2) std::swap(x1, x2);
    if (y1 > y2) std::swap(y1, y2);
}

Rectangle::Rectangle() : x1(0), y1(0), x2(0), y2(0) {}

Rectangle::Rectangle(int x1, int y1, int x2, int y2) : x1(x1), y1(y1), x2(x2), y2(y2) {
    normalize();
}

void Rectangle::getBottomLeft(int& x, int& y) const {
    x = x1;
    y = y1;
}

void Rectangle::getTopRight(int& x, int& y) const {
    x = x2;
    y = y2;
}

int Rectangle::getWidth() const {
    return x2 - x1;
}

int Rectangle::getHeight() const {
    return y2 - y1;
}

int Rectangle::area() const {
    return getWidth() * getHeight();
}

void Rectangle::move(int dx, int dy) {
    x1 += dx;
    y1 += dy;
    x2 += dx;
    y2 += dy;
}

void Rectangle::resize(int width, int height) {
    if (width < 0) width = 0;
    if (height < 0) height = 0;
    x2 = x1 + width;
    y2 = y1 + height;
}

Rectangle& Rectangle::operator++() {
    x2 += 1;
    y2 += 1;
    return *this;
}

Rectangle Rectangle::operator++(int) {
    Rectangle temp = *this;
    ++(*this);
    return temp;
}

Rectangle& Rectangle::operator--() {
    if (x2 - x1 > 0) x2 -= 1;
    if (y2 - y1 > 0) y2 -= 1;
    return *this;
}

Rectangle Rectangle::operator--(int) {
    Rectangle temp = *this;
    --(*this);
    return temp;
}

Rectangle Rectangle::operator+(const Rectangle& other) const {
    return Rectangle(
        std::min(x1, other.x1),
        std::min(y1, other.y1),
        std::max(x2, other.x2),
        std::max(y2, other.y2)
    );
}

Rectangle& Rectangle::operator+=(const Rectangle& other) {
    *this = *this + other;
    return *this;
}


Rectangle Rectangle::operator-(const Rectangle& other) const {
    int nx1 = std::max(x1, other.x1);
    int ny1 = std::max(y1, other.y1);
    int nx2 = std::min(x2, other.x2);
    int ny2 = std::min(y2, other.y2);

    if (nx1 <= nx2 && ny1 <= ny2) {
        return Rectangle(nx1, ny1, nx2, ny2);
    }
    return Rectangle(0, 0, 0, 0); 
}

Rectangle& Rectangle::operator-=(const Rectangle& other) {
    *this = *this - other;
    return *this;
}

bool Rectangle::intersects(const Rectangle& other) const {
    int nx1 = std::max(x1, other.x1);
    int ny1 = std::max(y1, other.y1);
    int nx2 = std::min(x2, other.x2);
    int ny2 = std::min(y2, other.y2);
    return (nx1 <= nx2 && ny1 <= ny2);
}

bool Rectangle::operator==(const Rectangle& other) const {
    return x1 == other.x1 && y1 == other.y1 && x2 == other.x2 && y2 == other.y2;
}

bool Rectangle::operator!=(const Rectangle& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Rectangle& r) {
    os << "Низ-Лево: (" << r.x1 << ", " << r.y1 << "), Верх-Право: (" << r.x2 << ", " << r.y2 << ")"
       << " [Ширина: " << r.getWidth() << ", Высота: " << r.getHeight() << "]";
    return os;
}

std::istream& operator>>(std::istream& is, Rectangle& r) {
    std::cout << "Введите левый нижний угол (x1 y1): ";
    is >> r.x1 >> r.y1;
    std::cout << "Введите правый верхний угол (x2 y2): ";
    is >> r.x2 >> r.y2;
    r.normalize();
    return is;
}