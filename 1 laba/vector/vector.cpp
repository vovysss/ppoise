
#include "vector.h"
#include <cmath>
#include <stdexcept>


Vector::Vector()
    : x1(0), y1(0), z1(0), x2(0), y2(0), z2(0) {}

Vector::Vector(double x1, double y1, double z1,
               double x2, double y2, double z2)
    : x1(x1), y1(y1), z1(z1), x2(x2), y2(y2), z2(z2) {}

Vector::Vector(const Vector& other)
    : x1(other.x1), y1(other.y1), z1(other.z1),
      x2(other.x2), y2(other.y2), z2(other.z2) {}

Vector& Vector::operator=(const Vector& other) {
    if (this != &other) {
        x1 = other.x1; y1 = other.y1; z1 = other.z1;
        x2 = other.x2; y2 = other.y2; z2 = other.z2;
    }
    return *this;
}

Vector::~Vector() = default;
 

double Vector::getX1() const { return x1; }
double Vector::getY1() const { return y1; }
double Vector::getZ1() const { return z1; }
double Vector::getX2() const { return x2; }
double Vector::getY2() const { return y2; }
double Vector::getZ2() const { return z2; }

 

void Vector::setX1(double x) { x1 = x; }
void Vector::setY1(double y) { y1 = y; }
void Vector::setZ1(double z) { z1 = z; }
void Vector::setX2(double x) { x2 = x; }
void Vector::setY2(double y) { y2 = y; }
void Vector::setZ2(double z) { z2 = z; }


namespace {
    double dx(const Vector& v) { return v.getX2() - v.getX1(); }
    double dy(const Vector& v) { return v.getY2() - v.getY1(); }
    double dz(const Vector& v) { return v.getZ2() - v.getZ1(); }

    Vector makeFromOrigin(double x, double y, double z) {
        return Vector(0, 0, 0, x, y, z);
    }
}
 

double Vector::length() const {
    return std::sqrt(dx(*this) * dx(*this) +
                     dy(*this) * dy(*this) +
                     dz(*this) * dz(*this));
}


bool Vector::operator==(const Vector& other) const {
    return std::abs(length() - other.length()) < 1e-9;
}
bool Vector::operator!=(const Vector& other) const { return !(*this == other); }
bool Vector::operator>(const Vector& other) const  { return length() > other.length(); }
bool Vector::operator>=(const Vector& other) const { return length() >= other.length(); }
bool Vector::operator<(const Vector& other) const  { return length() < other.length(); }
bool Vector::operator<=(const Vector& other) const { return length() <= other.length(); }

// Сложение

Vector Vector::operator+(const Vector& other) const {
    return makeFromOrigin(dx(*this) + dx(other),
                          dy(*this) + dy(other),
                          dz(*this) + dz(other));
}

Vector& Vector::operator+=(const Vector& other) {
    x2 += dx(other);
    y2 += dy(other);
    z2 += dz(other);
    return *this;
}


Vector Vector::operator-(const Vector& other) const {
    return makeFromOrigin(dx(*this) - dx(other),
                          dy(*this) - dy(other),
                          dz(*this) - dz(other));
}

Vector& Vector::operator-=(const Vector& other) {
    x2 -= dx(other);
    y2 -= dy(other);
    z2 -= dz(other);
    return *this;
}


Vector Vector::operator*(const Vector& other) const {
    double a = dy(*this) * dz(other) - dz(*this) * dy(other);
    double b = dz(*this) * dx(other) - dx(*this) * dz(other);
    double c = dx(*this) * dy(other) - dy(*this) * dx(other);
    return makeFromOrigin(a, b, c);
}

Vector& Vector::operator*=(const Vector& other) {
    *this = *this * other;
    return *this;
}


Vector Vector::operator*(double scalar) const {
    return makeFromOrigin(dx(*this) * scalar,
                          dy(*this) * scalar,
                          dz(*this) * scalar);
}

Vector& Vector::operator*=(double scalar) {
    x2 = x1 + dx(*this) * scalar;
    y2 = y1 + dy(*this) * scalar;
    z2 = z1 + dz(*this) * scalar;
    return *this;
}


Vector Vector::operator/(double scalar) const {
    if (std::abs(scalar) < 1e-9) {
        throw std::invalid_argument("Ошибка: деление вектора на нулевой скаляр");
    }
    return makeFromOrigin(dx(*this) / scalar,
                          dy(*this) / scalar,
                          dz(*this) / scalar);
}

Vector& Vector::operator/=(double scalar) {
    if (std::abs(scalar) < 1e-9) {
        throw std::invalid_argument("Ошибка: деление вектора на нулевой скаляр");
    }
    x2 = x1 + dx(*this) / scalar;
    y2 = y1 + dy(*this) / scalar;
    z2 = z1 + dz(*this) / scalar;
    return *this;
}


double Vector::operator^(const Vector& other) const {
    double dot = dx(*this) * dx(other) +
                 dy(*this) * dy(other) +
                 dz(*this) * dz(other);
    double lenProduct = length() * other.length();
    if (lenProduct < 1e-9) {
        throw std::invalid_argument("Ошибка: невозможно вычислить угол с вектором нулевой длины");
    }
    return dot / lenProduct;
}


std::ostream& operator<<(std::ostream& os, const Vector& v) {
    os << "(" << v.x1 << ", " << v.y1 << ", " << v.z1 << ") -> ("
       << v.x2 << ", " << v.y2 << ", " << v.z2 << ")";
    return os;
}

std::istream& operator>>(std::istream& is, Vector& v) {
    is >> v.x1 >> v.y1 >> v.z1
       >> v.x2 >> v.y2 >> v.z2;
    return is;
}

