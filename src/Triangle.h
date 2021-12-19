//
// Created by Martin Gano on 16.12.2021.
//

#ifndef RAY_TRACER_TRIANGLE_H
#define RAY_TRACER_TRIANGLE_H


#pragma once

#include "base.h"
#include "Vec.h"
#include <cmath>


class Triangle : public Primitive {
public:
    Triangle(Vec p0, Vec p1, Vec p2);

    Intersection intersect(Ray ray) override;

    void setMaterial(Material mat);

    const Vec &getP0() const;

    const Vec &getP1() const;

    const Vec &getP2() const;

private:
    Vec p0_{}, p1_{}, p2_{};

};

float getMin(int axis, Triangle &triangle);

float getMax(int axis, Triangle &triangle);

void print(Triangle &triangle);

#endif //RAY_TRACER_TRIANGLE_H
