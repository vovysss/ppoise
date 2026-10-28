#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <iostream>

class Rectangle {
private:
    int x1, y1; 
    int x2, y2; 

    void normalize();

public:
    Rectangle();
    Rectangle(int x1, int y1, int x2, int y2);

    void getBottomLeft(int& x, int& y) const;
    void getTopRight(int& x, int& y) const;
    int getWidth() const;
    int getHeight() const;
    int area() const; 


    void move(int dx, int dy);
    void resize(int width, int height);

    Rectangle& operator++();    
    Rectangle operator++(int);  
    Rectangle& operator--();    
    Rectangle operator--(int); 

    Rectangle operator+(const Rectangle& other) const;
    Rectangle& operator+=(const Rectangle& other);
    Rectangle operator-(const Rectangle& other) const;
    Rectangle& operator-=(const Rectangle& other);

    bool intersects(const Rectangle& other) const;

    bool operator==(const Rectangle& other) const;
    bool operator!=(const Rectangle& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Rectangle& r);
    friend std::istream& operator>>(std::istream& is, Rectangle& r);
};

#endif
