//
// Created by Martin Gano on 3.5.2022.
//

#include "Box.h"
#include <iostream>

using namespace std;

Box::Box(const cv::Vec3f& min_bound, const cv::Vec3f& max_bound) : min_bound_(min_bound), max_bound_(max_bound) {
}

cv::Vec3f Box::normalAt(cv::Vec3f point, cv::Vec3f min_bound, cv::Vec3f max_bound) {
    float EPS = 0.01;
    if (abs(point[1] - min_bound[1]) < EPS){
        return {0,-1,0};
    } else if(abs(point[0] - min_bound[0]) < EPS){
        return {-1,0,0};
    } else if(abs(point[2] - min_bound[2]) < EPS){
        return {0,0,-1};
    } else if (abs(point[1] - max_bound[1]) < EPS){
        return {0,1,0};
    } else if(abs(point[0] - max_bound[0]) < EPS){
        return {1,0,0};
    } else if(abs(point[2] - max_bound[2]) < EPS){
        return {0,0,1};
    } else{
        return {0,0,0};
    }
}

Intersection Box::intersect(Ray ray) {
    Intersection result;

    float t_min, t_max, ty_min, ty_max, tz_min, tz_max;

    int sigmoid_div_x[3];
    float inverse_direction_x = 1 / ray.direction_unit_v_[0];
    float inverse_direction_y = 1 / ray.direction_unit_v_[1];
    float inverse_direction_z = 1 / ray.direction_unit_v_[2];
    sigmoid_div_x[0] = (inverse_direction_x < 0);
    sigmoid_div_x[1] = (inverse_direction_y < 0);
    sigmoid_div_x[2] = (inverse_direction_z < 0);

    cv::Vec3f bounds[2];
    bounds[0] = min_bound_;
    bounds[1] = max_bound_;

    t_min = (bounds[sigmoid_div_x[0]][0] - ray.origin_c_[0]) * inverse_direction_x;
    t_max = (bounds[1-sigmoid_div_x[0]][0] - ray.origin_c_[0]) * inverse_direction_x;
    ty_min = (bounds[sigmoid_div_x[1]][1] - ray.origin_c_[1]) * inverse_direction_y;
    ty_max = (bounds[1-sigmoid_div_x[1]][1] - ray.origin_c_[1]) * inverse_direction_y;

    result.normal_unit_v_ = cv::Vec3f(-1,0,0);
    if ((t_min > ty_max) || (ty_min > t_max))
        return result;
    if (ty_min > t_min){
        t_min = ty_min;
        result.normal_unit_v_ = cv::Vec3f(0,-1,0);
    }
    if (ty_max < t_max)
        t_max = ty_max;

    tz_min = (bounds[sigmoid_div_x[2]][2] - ray.origin_c_[2]) * inverse_direction_z;
    tz_max = (bounds[1-sigmoid_div_x[2]][2] - ray.origin_c_[2]) * inverse_direction_z;

    if ((t_min > tz_max) || (tz_min > t_max))
        return result;
    if (tz_min > t_min){
        t_min = tz_min;
        result.normal_unit_v_ = cv::Vec3f(0,0,-1);
    }
    if (tz_max < t_max)
        t_max = tz_max;
    float t = t_min;
    cv::Vec3f P = ray.origin_c_ + ray.direction_unit_v_ * t;
    result.exists = true;
    result.normal_unit_v_ = Box::normalAt(P, min_bound_, max_bound_);
    result.contact_c_ = P;
    return result;
}


void Box::setMaterial(Material mat) {
    material_ = mat;
}

const cv::Vec3f &Box::getMinBound() const {
    return min_bound_;
}

void Box::setMinBound(const cv::Vec3f &minBound) {
    min_bound_ = minBound;
}

const cv::Vec3f &Box::getMaxBound() const {
    return max_bound_;
}

void Box::setMaxBound(const cv::Vec3f &maxBound) {
    max_bound_ = maxBound;
}
