//
// Created by Martin Gano on 3.5.2022.
//

#ifndef RAY_TRACER_SOLUTION_SCENE_H
#define RAY_TRACER_SOLUTION_SCENE_H

#include "KDTree.h"
#include "base.h"
#include "Image.h"
#include <cstdlib>
#include <iostream>

class Scene {
public:
    KDTree *kd_tree{};

    Color rayTracing(Ray ray, vector<cv::Vec3f> &vertexes);

    void render(Image *image, vector<cv::Vec3f> &vertexes);

    Camera *camera{};

    std::vector<Light *> lights;

    void setAmbientColor(const Color &ambientColor);

private:
    Color ambient_color_;

};


#endif //RAY_TRACER_SOLUTION_SCENE_H
