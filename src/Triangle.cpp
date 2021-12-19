//
// Created by Martin Gano on 16.12.2021.
//

#include "Triangle.h"
#include <iostream>

using namespace std;

/*
 * Determines if the vector is perpendicular to the triangle's plane
 */
bool insideOutsideTest(Vec t0, Vec t1, Vec p0, Vec p1) {
    return ((p1 - p0).cross(t0 - p0)).dot((p1 - p0).cross(t1 - p0)) >= 0;
}

Triangle::Triangle(Vec p0, Vec p1, Vec p2)
        : p0_(p0), p1_(p1), p2_(p2) {
}

/*
 * Solving parametric equation similar as for plane: P = O + t*R;
 * where P is the intersection, O is the ray origin and R is the ray direction (vector)
 * Theoretical background adapted from:
 * https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution
 *
 *
 * D - distance from the origin to plane
 * float D = N.x * v0.x + N.y * v0.y + N.z * v0.z;
 * t = - (dot(N, orig) + D) / dot(N, dir);
 * P = orig + t * dir;
 */
Intersection Triangle::intersect(Ray ray) {
    Intersection contact;
    contact.exists = false;
    Vec u = p1_ - p0_;
    Vec v = p2_ - p0_;
    Vec normal = u.cross(v);
    Vec p0_to_ray = ray.origin_c_ - p0_;
    float dir;
    float denominator = normal.dot(ray.direction_unit_v_);
    dir = -normal.dot(p0_to_ray) / denominator;
    bool not_intersect = abs(denominator) < 0.0001 or dir < 0;
    if (not_intersect) {
        return contact;
    }
    contact.contact_c_ = ray.origin_c_ + ray.direction_unit_v_ * dir;
    Vec w = contact.contact_c_ - p0_;
    bool is_inside = insideOutsideTest(w, u, v, Vec(0, 0, 0))
                     && insideOutsideTest(w, Vec(0, 0, 0), u, v)
                     && insideOutsideTest(w, v, Vec(0, 0, 0), u);
    if (is_inside) {
        contact.exists = true;
        contact.normal_unit_v_ = normal.unit();
    }

    return contact;
}


void Triangle::setMaterial(Material mat) {
    material_ = mat;
}

const Vec &Triangle::getP0() const {
    return p0_;
}

const Vec &Triangle::getP1() const {
    return p1_;
}

const Vec &Triangle::getP2() const {
    return p2_;
}

float max(float n1, float n2) {
    return n1 < n2 ? n2 : n1;
}

float getMax(int axis, Triangle &triangle) {
    //0 ~ x
    //1 ~ y
    //2 ~ y
    if (axis == 0) {
        return max(triangle.getP0().x(), max(triangle.getP1().x(), triangle.getP2().x()));
    } else if (axis == 1) {
        return max(triangle.getP0().y(), max(triangle.getP1().y(), triangle.getP2().y()));
    } else if (axis == 2) {
        return max(triangle.getP0().z(), max(triangle.getP1().z(), triangle.getP2().z()));
    }
    cerr << "Wrong axis_!" << endl;
    return 0.0;
}

float getMin(int axis, Triangle &triangle) {
    //0 ~ x
    //1 ~ y
    //2 ~ y
    if (axis == 0) {
        return min(triangle.getP0().x(), min(triangle.getP1().x(), triangle.getP2().x()));
    } else if (axis == 1) {
        return min(triangle.getP0().y(), min(triangle.getP1().y(), triangle.getP2().y()));
    } else if (axis == 2) {
        return min(triangle.getP0().z(), min(triangle.getP1().z(), triangle.getP2().z()));
    }
    cerr << "Wring axis_!" << endl;
    return 0.0;
}

void print(Triangle& triangle){
    cout << triangle.getP0().x() << " " << triangle.getP0().y() << " " << triangle.getP0().z() << endl;
    cout << triangle.getP1().x() << " " << triangle.getP1().y() << " " << triangle.getP1().z() << endl;
    cout << triangle.getP2().x() << " " << triangle.getP2().y() << " " << triangle.getP2().z() << endl;
}