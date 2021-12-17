//
// Created by Martin Gano on 16.12.2021.
//

#ifndef FINAL_SOLUTION_TRIANGLE_H
#define FINAL_SOLUTION_TRIANGLE_H


#pragma once

#include "base.h"
#include "Vec.h"
#include <cmath>

class Triangle : public Primitive {
public:
    Triangle(Vec p0, Vec p1, Vec p2);

    Intersection intersect(Ray ray) override;

    void setMaterial(Material mat);

private:
    Vec p0_{}, p1_{}, p2_{};
};


#endif //FINAL_SOLUTION_TRIANGLE_H
