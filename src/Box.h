//
// Created by Martin Gano on 3.5.2022.
// the main reason for implementing this Primitive is to enable splitting space box into subboxes
// and eventually construct kd-tree
//

#ifndef RAYTRACER_BOX_H
#define RAYTRACER_BOX_H

#include "base.h"


class Box {
public:
    Box(const cv::Vec3f& min_bound, const cv::Vec3f& max_bound);
    Intersection intersect(Ray ray);
    void setMaterial(Material mat);

    const cv::Vec3f &getMinBound() const;

    void setMinBound(const cv::Vec3f &minBound);

    const cv::Vec3f &getMaxBound() const;

    void setMaxBound(const cv::Vec3f &maxBound);

private:

    static cv::Vec3f normalAt(cv::Vec3f point, cv::Vec3f min_bound, cv::Vec3f max_bound);
    cv::Vec3f min_bound_{}, max_bound_{};
    Material material_;
};


#endif //RAYTRACER_BOX_H
