//
// Created by Martin Gano on 18.12.2021.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "base.h"
#include "Vec.h"

class Sphere : public Primitive {
public:
    Sphere(Vec origin_c, float radius);
    Intersection intersect(Ray ray) override;
    void setMaterial(Material mat);

private:
    Vec  origin_c_{};
    float radius_{};

};


#endif //RAYTRACER_SPHERE_H
