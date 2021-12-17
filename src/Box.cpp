//
// Created by Martin Gano on 17.12.2021.
//

#include "Box.h"
#include <iostream>

using namespace std;

Box::Box(Vec min_bound, Vec max_bound) : min_bound_(min_bound), max_bound_(max_bound) {
}

Vec Box::normalAt(Vec point, Vec min_bound, Vec max_bound) {
    float EPS = 0.00001;
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
        cout << "Error" << endl;
        return {0,0,0};
    }
}

Intersection Box::intersect(Ray ray) {
    Intersection result;

    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    int sigmodivx[3];
    float invdir_x = 1 / ray.direction_unit_v_.x();
    float invdir_y = 1 / ray.direction_unit_v_.y();
    float invdir_z = 1 / ray.direction_unit_v_.z();
    sigmodivx[0] = (invdir_x < 0);
    sigmodivx[1] = (invdir_y < 0);
    sigmodivx[2] = (invdir_z < 0);

    Vec bounds[2];
    bounds[0] = min_bound_;
    bounds[1] = max_bound_;

    tmin = (bounds[sigmodivx[0]].x() - ray.origin_c_.x()) * invdir_x;
    tmax = (bounds[1-sigmodivx[0]].x() - ray.origin_c_.x()) * invdir_x;
    tymin = (bounds[sigmodivx[1]].y() - ray.origin_c_.y()) * invdir_y;
    tymax = (bounds[1-sigmodivx[1]].y() - ray.origin_c_.y()) * invdir_y;

    result.normal_unit_v_ = Vec(-1,0,0);
    if ((tmin > tymax) || (tymin > tmax))
        return result;
    if (tymin > tmin){
        tmin = tymin;
        result.normal_unit_v_ = Vec(0,-1,0);
    }

    if (tymax < tmax)
        tmax = tymax;

    tzmin = (bounds[sigmodivx[2]].z() - ray.origin_c_.z()) * invdir_z;
    tzmax = (bounds[1-sigmodivx[2]].z() - ray.origin_c_.z()) * invdir_z;

    if ((tmin > tzmax) || (tzmin > tmax))
        return result;
    if (tzmin > tmin){
        tmin = tzmin;
        result.normal_unit_v_ = Vec(0,0,-1);
    }

    if (tzmax < tmax)
        tmax = tzmax;

    float t = tmin;
    Vec P = ray.origin_c_ + ray.direction_unit_v_ * t;


    result.exists = true;
    result.normal_unit_v_ = Box::normalAt(P, min_bound_, max_bound_);


    result.contact_c_ = P;

    return result;
}


void Box::setMaterial(Material mat) {
    material_ = mat;
}
