//
// Created by Martin Gano on 3.5.2022.
//

#include "Triangle.h"

using namespace std;

float vec_length(const cv::Vec3f &vector_) {
    return sqrt(vector_.val[0] * vector_.val[0] + vector_.val[1] * vector_.val[1] + vector_.val[2] * vector_.val[2]);
}

cv::Vec3f unit_vec(const cv::Vec3f &vector_) {
    float vector_length = vec_length(vector_);
    return {vector_.val[0] / vector_length,
            vector_.val[1] / vector_length,
            vector_.val[2] / vector_length};
}


/*
 * Determines if the vector is perpendicular to the triangle's plane
 */
bool insideOutsideTest(const cv::Vec3f& t0, const cv::Vec3f& t1, const cv::Vec3f& p0, const cv::Vec3f& p1) {
    return ((p1 - p0).cross(t0 - p0)).dot((p1 - p0).cross(t1 - p0)) >= 0;
}

Triangle::Triangle(const long p0, const long p1, const long p2)
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
Intersection Triangle::intersect(Ray ray, vector<cv::Vec3f> &vertexes) {
    total_intersections++;

    Intersection contact;
    contact.exists = false;
    cv::Vec3f u = vertexes[p1_] - vertexes[p0_];
    cv::Vec3f v = vertexes[p2_] - vertexes[p0_];
    cv::Vec3f normal = u.cross(v);
    cv::Vec3f p0_to_ray = ray.origin_c_ - vertexes[p0_];
    float dir;
    float denominator = normal.dot(ray.direction_unit_v_);
    dir = -normal.dot(p0_to_ray) / denominator;
    bool not_intersect = abs(denominator) < 0.0001 or dir < 0;
    if (not_intersect) {
        return contact;
    }
    contact.contact_c_ = ray.origin_c_ + ray.direction_unit_v_ * dir;
    cv::Vec3f w = contact.contact_c_ - vertexes[p0_];
    bool is_inside = insideOutsideTest(w, u, v, cv::Vec3f(0, 0, 0))
                     && insideOutsideTest(w, cv::Vec3f(0, 0, 0), u, v)
                     && insideOutsideTest(w, v, cv::Vec3f(0, 0, 0), u);
    if (is_inside) {
        contact.exists = true;
        contact.normal_unit_v_ = unit_vec(normal);
    }

    return contact;
}


void Triangle::setMaterial(Material mat) {
    material_ = mat;
}

const cv::Vec3f &Triangle::getP0(vector<cv::Vec3f> &vertexes) const {
    return vertexes[p0_];
}

const cv::Vec3f &Triangle::getP1(vector<cv::Vec3f> &vertexes) const {
    return vertexes[p1_];
}

const cv::Vec3f &Triangle::getP2(vector<cv::Vec3f> &vertexes) const {
    return vertexes[p2_];
}

float max(float n1, float n2) {
    return n1 < n2 ? n2 : n1;
}

