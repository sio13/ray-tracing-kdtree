//
// Created by Martin Gano on 16.12.2021.
//

#ifndef FINAL_SOLUTION_PLANE_H
#define FINAL_SOLUTION_PLANE_H


#include "base.h"
#include "Vec.h"

class Plane : public Primitive {
public:
    Plane(Vec origin_c, Vec normal_unit_v);

    Intersection intersect(Ray ray) override;
    void setMaterial(Material mat);

private:
    Vec origin_c;
    Vec normal_unit_v_;
};

#endif //FINAL_SOLUTION_PLANE_H
