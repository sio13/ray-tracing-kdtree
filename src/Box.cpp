//
// Created by Martin Gano on 17.12.2021.
//

#include "Box.h"
#include <iostream>

using namespace std;

Box::Box(Vec min_bound, Vec max_bound) : min_bound_(min_bound), max_bound_(max_bound) {
}

Vec Box::normalAt(Vec point, Vec min_bound, Vec max_bound) {
    float EPS = 0.01;
    if (abs(point.y() - min_bound.y()) < EPS){
        return {0,-1,0};
    } else if(abs(point.x() - min_bound.x()) < EPS){
        return {-1,0,0};
    } else if(abs(point.z() - min_bound.z()) < EPS){
        return {0,0,-1};
    } else if (abs(point.y() - max_bound.y()) < EPS){
        return {0,1,0};
    } else if(abs(point.x() - max_bound.x()) < EPS){
        return {1,0,0};
    } else if(abs(point.z() - max_bound.z()) < EPS){
        return {0,0,1};
    } else{
        return {0,0,0};
    }
}

Intersection Box::intersect(Ray ray) {
    Intersection result;

    float t_min, t_max, ty_min, ty_max, tz_min, tz_max;

    int sigmoid_div_x[3];
    float inverse_direction_x = 1 / ray.direction_unit_v_.x();
    float inverse_direction_y = 1 / ray.direction_unit_v_.y();
    float inverse_direction_z = 1 / ray.direction_unit_v_.z();
    sigmoid_div_x[0] = (inverse_direction_x < 0);
    sigmoid_div_x[1] = (inverse_direction_y < 0);
    sigmoid_div_x[2] = (inverse_direction_z < 0);

    Vec bounds[2];
    bounds[0] = min_bound_;
    bounds[1] = max_bound_;

    t_min = (bounds[sigmoid_div_x[0]].x() - ray.origin_c_.x()) * inverse_direction_x;
    t_max = (bounds[1-sigmoid_div_x[0]].x() - ray.origin_c_.x()) * inverse_direction_x;
    ty_min = (bounds[sigmoid_div_x[1]].y() - ray.origin_c_.y()) * inverse_direction_y;
    ty_max = (bounds[1-sigmoid_div_x[1]].y() - ray.origin_c_.y()) * inverse_direction_y;

    result.normal_unit_v_ = Vec(-1,0,0);
    if ((t_min > ty_max) || (ty_min > t_max))
        return result;
    if (ty_min > t_min){
        t_min = ty_min;
        result.normal_unit_v_ = Vec(0,-1,0);
    }
    if (ty_max < t_max)
        t_max = ty_max;

    tz_min = (bounds[sigmoid_div_x[2]].z() - ray.origin_c_.z()) * inverse_direction_z;
    tz_max = (bounds[1-sigmoid_div_x[2]].z() - ray.origin_c_.z()) * inverse_direction_z;

    if ((t_min > tz_max) || (tz_min > t_max))
        return result;
    if (tz_min > t_min){
        t_min = tz_min;
        result.normal_unit_v_ = Vec(0,0,-1);
    }
    if (tz_max < t_max)
        t_max = tz_max;
    float t = t_min;
    Vec P = ray.origin_c_ + ray.direction_unit_v_ * t;
    result.exists = true;
    result.normal_unit_v_ = Box::normalAt(P, min_bound_, max_bound_);
    result.contact_c_ = P;
    return result;
}


void Box::setMaterial(Material mat) {
    material_ = mat;
}

const Vec &Box::getMinBound() const {
    return min_bound_;
}

void Box::setMinBound(const Vec &minBound) {
    min_bound_ = minBound;
}

const Vec &Box::getMaxBound() const {
    return max_bound_;
}

void Box::setMaxBound(const Vec &maxBound) {
    max_bound_ = maxBound;
}
