//
// Created by Martin Gano on 3.5.2022.
//

#ifndef RAY_TRACER_KDTREE_H
#define RAY_TRACER_KDTREE_H

#include <vector>
#include "base.h"
#include "Box.h"
#include "Triangle.h"
#include "defaults.h"

using namespace std;

float getMin(int axis, const Triangle &triangle);

float getMax(int axis, const Triangle &triangle);


class KDTree {
public:

    vector<Triangle *> objects;

    static KDTree *construct_kdtree(vector<Triangle *> &objects, int level, vector<cv::Vec3f> &vertexes);

    bool get_intersection(const Ray& ray, Triangle **object, Intersection *intersection, vector<cv::Vec3f> &vertexes);

    static bool traverse_tree(KDTree *node, Ray &ray, Triangle **object, Intersection *intersection, vector<cv::Vec3f> &vertexes);

    Box getInnerSpaceBox();

    KDTree(const vector<Triangle *> &objects, Box innerSpaceBox, int axis, KDTree *left, KDTree *right,
           double split);

    KDTree *getLeft() const;

    void setLeft(KDTree *left);

    KDTree *getRight() const;

    void setRight(KDTree *right);

    double getSplit() const;

    void setSplit(double split);

    void setLevel(int level);


private:
    Box inner_space_box_;
    int axis_;
    KDTree *left_;
    KDTree *right_;
    double split_;
    int level_{};


};


#endif //RAY_TRACER_KDTREE_H
