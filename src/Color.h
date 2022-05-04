//
// Created by Martin Gano on 3.5.2022.
//

#ifndef RAY_TRACER_COLOR_H
#define RAY_TRACER_COLOR_H

#include <cmath>


class Color {
public:
    Color(float r, float g, float b)
            : r_(r), g_(g), b_(b) {}
    Color() : r_(0), g_(0), b_(0) {}

    float r() const { return r_; }
    float g() const { return g_; }
    float b() const { return b_; }

    void clip();
    void gammaCorrect();

    Color operator+=(const Color& other);
    Color operator+(const Color& other) const;
    Color operator*(float k) const;
    Color operator*(const Color& other) const;
    Color operator/(float k) const;

private:
    static float lu(float c);
    float r_, g_, b_;
};

#endif //RAY_TRACER_COLOR_H
