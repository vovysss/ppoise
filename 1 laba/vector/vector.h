
#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <cmath>

class Vector {
private:
    double x1, y1, z1; 
    double x2, y2, z2; 

public:
    Vector();
    Vector(double x1, double y1, double z1, double x2, double y2, double z2);
    Vector(const Vector& other);
    Vector& operator=(const Vector& other);
    ~Vector();


    double getX1() const;
    double getY1() const;
    double getZ1() const;
    double getX2() const;
    double getY2() const;
    double getZ2() const;


    void setX1(double x);
    void setY1(double y);
    void setZ1(double z);
    void setX2(double x);
    void setY2(double y);
    void setZ2(double z);

  
    double length() const;

    
    bool operator==(const Vector& other) const;
    bool operator!=(const Vector& other) const;
    bool operator>(const Vector& other) const;
    bool operator>=(const Vector& other) const;
    bool operator<(const Vector& other) const;
    bool operator<=(const Vector& other) const;

    
    Vector operator+(const Vector& other) const;
    Vector& operator+=(const Vector& other);

    Vector operator-(const Vector& other) const;
    Vector& operator-=(const Vector& other);

    
    Vector operator*(const Vector& other) const;
    Vector& operator*=(const Vector& other);

   
    Vector operator*(double scalar) const;
    Vector& operator*=(double scalar);

    /// @brief Деление на скаляр
    Vector operator/(double scalar) const;
    Vector& operator/=(double scalar);

    
    double operator^(const Vector& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Vector& v);
    friend std::istream& operator>>(std::istream& is, Vector& v);
};

#endif 