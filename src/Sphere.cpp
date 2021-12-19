//
// Created by Martin Gano on 18.12.2021.
//

#include "Sphere.h"

Sphere::Sphere(Vec origin_c, float radius)
        : origin_c_(origin_c), radius_(radius) {
}

/*
 * To find intersection of the vector and the sphere we have to resolve quadratic equation
 * The solution is adapted from
 * https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
 */
Intersection Sphere::intersect(Ray ray) {
    Intersection contact;
    float a, b, c, D;
    a = ray.direction_unit_v_.dot(ray.direction_unit_v_);
    b = ray.direction_unit_v_.dot((ray.origin_c_ - origin_c_) * 2);
    c = origin_c_.dot(origin_c_) + ray.origin_c_.dot(ray.origin_c_) - ray.origin_c_.dot(origin_c_) * 2 -
        radius_ * radius_;
    //discriminant
    D = b * b - a * c * 4;
    contact.exists = false;
    if (D >= 0) {
        float t = (float) -0.5 * (b + sqrt(D)) / a;
        if (t > 0) {
            Vec contact_point = ray.origin_c_ + ray.direction_unit_v_ * t;
            contact.contact_c_ = contact_point;
            contact.normal_unit_v_ = (contact_point - origin_c_) / radius_;;
            contact.exists = true;
        }
    }

    return contact;
}

void Sphere::setMaterial(Material mat) {
    material_ = mat;
}