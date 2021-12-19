//
// Created by Martin Gano on 17.12.2021.
//

#ifndef RAY_TRACER_KDTREE_H
#define RAY_TRACER_KDTREE_H

#include <vector>
#include "base.h"
#include "Box.h"
#include "Triangle.h"

using namespace std;


class KDTree {
public:

    vector<Triangle *> objects;

    static KDTree *construct_kdtree(vector<Triangle *> &objects, unsigned int &max_objects, int level);

    bool get_intersection(Ray ray, Triangle **object, Intersection *intersection);

    static Intersection traverse_tree(KDTree *node, Ray &ray);

    Box getInnerSpaceBox();

    void setInnerSpaceBox(const Box &innerSpaceBox);

    KDTree(const vector<Triangle *> &objects, Box innerSpaceBox, int axis, KDTree *left, KDTree *right,
           double split);

    int getAxis() const;

    void setAxis(int axis);

    KDTree *getLeft() const;

    void setLeft(KDTree *left);

    KDTree *getRight() const;

    void setRight(KDTree *right);

    double getSplit() const;

    void setSplit(double split);


private:
    Box inner_space_box_;
    int axis_;
    KDTree *left_;
    KDTree *right_;
    double split_;

};


#endif //RAY_TRACER_KDTREE_H
