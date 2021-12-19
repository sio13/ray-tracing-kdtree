//
// Created by Martin Gano on 16.12.2021.
//


#include "Vec.h"


using namespace std;

Vec Vec::operator+=(const Vec &other) {
    x_ += other.x_;
    y_ += other.y_;
    z_ += other.z_;
    return *this;
}

Vec Vec::operator-=(const Vec &other) {
    x_ -= other.x_;
    y_ -= other.y_;
    z_ -= other.z_;
    return *this;
}

Vec Vec::operator+(const Vec &other) const {
    return Vec(*this) += other;
}

Vec Vec::operator-(const Vec &other) const {
    return Vec(*this) -= other;
}

Vec Vec::operator-() const {
    return {-x_, -y_, -z_};
}

Vec Vec::operator*(float k) const {
    return {x_ * k, y_ * k, z_ * k};
}

Vec Vec::operator/(float k) const {
    return {x_ / k, y_ / k, z_ / k};
}

float Vec::length() const {
    return sqrt(x_ * x_ + y_ * y_ + z_ * z_);
}

Vec Vec::unit() const {
    float len = length();
    return {x_ / len, y_ / len, z_ / len};
}

//scalar product - not normalized cos - to normalize - unit()
float Vec::dot(const Vec &other) const {
    return x_ * other.x_ + y_ * other.y_ + z_ * other.z_;
}

Vec Vec::cross(const Vec &other) const {
    return {y_ * other.z_ - z_ * other.y_,
            z_ * other.x_ - x_ * other.z_,
            x_ * other.y_ - y_ * other.x_};
}

void Vec::setX(float x) {
    x_ = x;
}

void Vec::setY(float y) {
    y_ = y;
}

void Vec::setZ(float z) {
    z_ = z;
}
