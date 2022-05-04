//
// Created by Martin Gano on 3.5.2022.
//

#include "Color.h"



float Color::lu(float c) {
    // adapted from http://www.w3.org/Graphics/Color/srgb
    if (c <= 0.0031308f)
        return 12.92f * c;
    else
        return 1.055f * powf(c, 0.4166667f) - 0.055f;
}

void Color::gammaCorrect() {
    r_ = lu(r_);
    g_ = lu(g_);
    b_ = lu(b_);
}

float min(float n1, float n2) {
    return n1 > n2 ? n2 : n1;
}

void Color::clip() {
    r_ = min(1, r_);
    g_ = min(1, g_);
    b_ = min(1, b_);
}

//overloading of the operators from here down
Color Color::operator+=(const Color& other) {
    r_ += other.r_;
    g_ += other.g_;
    b_ += other.b_;
    return *this;
}


Color Color::operator*(const Color& other) const {
    return {r_ * other.r_, g_ * other.g_, b_ * other.b_};
}

Color Color::operator*(float k) const {
    return {r_ * k, g_ * k, b_ * k};
}

Color Color::operator+(const Color& other) const {
    return Color(*this) += other;
}

Color Color::operator / (float k) const {
    return {r_ / k, g_ / k, b_ / k};
}
