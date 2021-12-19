//
// Created by Martin Gano on 17.12.2021.
// quality == suffix '_q'
//

#ifndef RAY_TRACER_SOLUTION_SCENE_H
#define RAY_TRACER_SOLUTION_SCENE_H

#include "KDTree.h"
#include "base.h"
#include "Vec.h"
#include "Image.h"
#include <cstdlib>
#include <iostream>

class Scene {
public:
    KDTree *kd_tree{};

    Color traceRay(Ray ray, int reflection_count = 0);

    void render(Image *image);

    Camera *camera{};

    std::vector<Light *> lights;


    void setSoftlightQ(int softlightQ);


    void setOcclusionQ(int occlusionQ);

    void setAmbientColor(const Color &ambientColor);

    void setMaxReflectionsCount(int maxReflectionsCount); // used for recursive tracing of the ray

private:
    Color ambient_color_;
    int softlight_q{};
    int occlusion_q_{};
    int max_reflections_count_{};

};


#endif //RAY_TRACER_SOLUTION_SCENE_H
