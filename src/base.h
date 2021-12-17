//
// Created by Martin Gano on 16.12.2021.
//
// coordinates == suffix '_c'
// vector == suffix '_v'
// coefficient == suffix '_k
// color_ == suffix '_f' == from German die Farbe

#ifndef RAY_TRACER_BASE_H
#define RAY_TRACER_BASE_H

#include "Vec.h"
#include "Color.h"

struct Material {
    float ambient_k_{}, diffuse_k_{}, occlusion_k{}, reflection_k{}, specular_k_{}, shininess_{};
    Color diffuse_f_, reflection_f, specular_f_;
};

struct Ray {
    Ray() = default;

    Ray(Vec origin_c, Vec direction_unit_v)
            : origin_c_(origin_c), direction_unit_v_(direction_unit_v) {}

    Ray reflect(Vec axis_unit_v) const;

    Vec origin_c_;
    Vec direction_unit_v_;
};


struct Intersection {
    bool exists{};
    Vec contact_c_;
    Vec normal_unit_v_;
};

class Primitive {
public:
    Material material_{};

    virtual Intersection intersect(Ray ray) = 0;
};

struct Light {
    Vec origin_c_;
    Color color_;
    float intensity_{};
    float radius{};
};

struct Camera {
    Vec origin_c;
    Vec view_plane_point_c;
    float view_plane_width{};
    float view_plane_height{};
};


#endif //RAY_TRACER_BASE_H
