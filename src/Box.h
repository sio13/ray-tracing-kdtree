//
// Created by Martin Gano on 17.12.2021.
// the main reason for implementing this Primitive is to enable splitting space box into subboxes
// and eventually construct kd-tree
//

#ifndef RAYTRACER_BOX_H
#define RAYTRACER_BOX_H

#include "base.h"


class Box : public Primitive {
public:
    Box(Vec min_bound, Vec max_bound);
    Intersection intersect(Ray ray) override;
    void setMaterial(Material mat);

private:
    Vec min_bound_{}, max_bound_{};

    static Vec normalAt(Vec point, Vec min_bound, Vec max_bound);
};


#endif //RAYTRACER_BOX_H
