//
// Created by Martin Gano on 3.5.2022.
//

#ifndef RAY_TRACER_TRIANGLE_H
#define RAY_TRACER_TRIANGLE_H


#pragma once

#include "base.h"
#include <cmath>
#include <opencv2/core/matx.hpp>

float vec_length(const cv::Vec3f& vector_);
cv::Vec3f unit_vec(const cv::Vec3f &vector_);


class Triangle : public Primitive {
public:

    static unsigned long long total_intersections;

    Triangle(long p0, long p1, long p2);

    Intersection intersect(Ray ray, vector<cv::Vec3f> &vertexes) override;

    void setMaterial(Material mat);

    const cv::Vec3f &getP0(vector<cv::Vec3f> &vertexes) const;

    const cv::Vec3f &getP1(vector<cv::Vec3f> &vertexes) const;

    const cv::Vec3f &getP2(vector<cv::Vec3f> &vertexes) const;

private:
    int p0_{}, p1_{}, p2_{};

};

#endif //RAY_TRACER_TRIANGLE_H
