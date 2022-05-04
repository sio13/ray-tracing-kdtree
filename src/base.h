//
// Created by Martin Gano on 3.5.2022.
//


#ifndef RAY_TRACER_BASE_H
#define RAY_TRACER_BASE_H

#include "Color.h"
#include <vector>
#include <opencv2/core/matx.hpp>

using namespace std;

struct Material {
    float ambient_k_{}, diffuse_k_{}, specular_k_{}, shininess_{};
    Color diffuse_f_, specular_f_;
};

struct Ray {
    Ray() = default;

    Ray(const cv::Vec3f& origin_c, const cv::Vec3f& direction_unit_v)
            : origin_c_(origin_c), direction_unit_v_(direction_unit_v) {}

    Ray reflect(const cv::Vec3f& axis_unit_v) const;

    cv::Vec3f origin_c_;
    cv::Vec3f direction_unit_v_;
};


struct Intersection {
    bool exists{};
    cv::Vec3f contact_c_;
    cv::Vec3f normal_unit_v_;
};

class Primitive {
public:
    Material material_{};

    virtual Intersection intersect(Ray ray, vector<cv::Vec3f> &vertexes) = 0;
};

struct Light {
    cv::Vec3f origin_c_;
    Color color_;
    float intensity_{};
};

struct Camera {
    cv::Vec3f origin_c;
    cv::Vec3f view_plane_point_c;
    float view_plane_width{};
    float view_plane_height{};
};


#endif //RAY_TRACER_BASE_H
