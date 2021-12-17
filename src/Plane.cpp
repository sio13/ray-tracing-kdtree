//
// Created by Martin Gano on 16.12.2021.
//
#include "Plane.h"
#include <iostream>

using namespace std;


Plane::Plane(Vec origin_c, Vec normal_unit_v)
        : origin_c(origin_c), normal_unit_v_(normal_unit_v) {
}

/*
 * This method computes intersection of ray with plane if exists.
 * The attribute of the returned instance of Intersection contain info.
 * This method assumes vectors are all normalized.
 * Equation adapted from
 * https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
 * parameter t = ((p_0 - l_0) dot n) / (l dot n)
 * where p_0 - plane origin
 *       l_0 - ray origin
 *       p_0 - l_0 - distance from ray source to plane
 *       l - ray direction (ray vector)
 *       n - plane normal
 */
Intersection Plane::intersect(Ray ray) {

    Intersection contact;
    contact.exists = false;
    float denominator = ray.direction_unit_v_.dot(normal_unit_v_);
    float t = (origin_c - ray.origin_c_).dot(normal_unit_v_) / denominator;
    bool not_intersect = t < 0 or abs(denominator) < 0.001;
    if (not_intersect) {
        return contact;
    }
    contact.exists = true;
    contact.normal_unit_v_ = normal_unit_v_;
    contact.contact_c_ = ray.direction_unit_v_ * t + ray.origin_c_;

    return contact;
}


void Plane::setMaterial(Material mat){
    material_ = mat;
}