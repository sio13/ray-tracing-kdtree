//
// Created by Martin Gano on 3.5.2022.
//

#include "KDTree.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <opencv2/core/matx.hpp>

using namespace std;

vector<cv::Vec3f> vertexes_;

KDTree::KDTree(const vector<Triangle *> &objects, Box innerSpaceBox, int axis, KDTree *left, KDTree *right,
               double split) : objects(objects), inner_space_box_(std::move(innerSpaceBox)), axis_(axis), left_(left),
                               right_(right), split_(split) {}


float getMax(int axis, const Triangle &triangle) {
    return max(triangle.getP0(vertexes_)[axis], max(triangle.getP1(vertexes_)[axis], triangle.getP2(vertexes_)[axis]));
}

float getMin(int axis, const Triangle &triangle) {
    return min(triangle.getP0(vertexes_)[axis], min(triangle.getP1(vertexes_)[axis], triangle.getP2(vertexes_)[axis]));
}

bool compare_x(Triangle *i1, Triangle *i2){
    return getMin(0, *i1) < getMin(0, *i2);
}

bool compare_y(Triangle *i1, Triangle *i2){
    return getMin(1, *i1) < getMin(1, *i2);
}

bool compare_z(Triangle *i1, Triangle *i2){
    return getMin(2, *i1) < getMin(2, *i2);
}

bool isLeave(KDTree *node) {
    return node->getLeft() == nullptr && node->getRight() == nullptr;
}

KDTree *KDTree::construct_kdtree(vector<Triangle *> &objects, int level, vector<cv::Vec3f> &vertexes) {

    vertexes_ = vertexes;

    cv::Vec3f max_bound = cv::Vec3f(objects[0]->getP0(vertexes)[0], objects[0]->getP0(vertexes)[1], objects[0]->getP0(vertexes)[2]);
    cv::Vec3f min_bound = cv::Vec3f(objects[0]->getP0(vertexes)[0], objects[0]->getP0(vertexes)[1], objects[0]->getP0(vertexes)[2]);

    for (auto &object : objects) {
        for (int ax = 0; ax < 3; ++ax) {
            if (getMin(ax, *object) < min_bound[ax]) {
                min_bound[ax] = getMin(ax, *object);
            }
            if (getMax(ax, *object) > max_bound[ax]) {
                max_bound[ax] = getMax(ax, *object);
            }
        }
    }

    Box inner_bound_box = Box(min_bound, max_bound);
    auto *new_tree = new KDTree(objects, inner_bound_box, level % 3, nullptr, nullptr, 0);

    if(!IDEAL_SPLIT_RAND){
        if(new_tree->axis_ == 0){
            sort(objects.begin(), objects.end(), compare_x);
        } else if(new_tree->axis_ == 1){
            sort(objects.begin(), objects.end(), compare_y);
        } else {
            sort(objects.begin(), objects.end(), compare_z);
        }

    }

    Triangle split = *objects[objects.size() / 2];
    double ideal_split;
    if (!IDEAL_SPLIT_MID){
        ideal_split = getMax(new_tree->axis_, split);
    } else {
        ideal_split = (getMin(new_tree->axis_, *objects[0]) + getMax(new_tree->axis_, *objects[objects.size() - 1])) / 2;
    }
    new_tree->setSplit(ideal_split);
    new_tree->objects = objects;
    new_tree->setLevel(level);

    if (level >= MAX_LEVEL || objects.size() <= MIN_OBJECTS) {
        new_tree->setRight(nullptr);
        new_tree->setLeft(nullptr);
        return new_tree;
    }

    vector<Triangle *> left;
    vector<Triangle *> right;

    for (auto &object : objects) {
        if (getMax(new_tree->axis_, *object) < new_tree->getSplit()) {
            left.push_back(object);
        } else if (getMin(new_tree->axis_, *object) > new_tree->getSplit()) {
            right.push_back(object);
        } else {
            left.push_back(object);
            right.push_back(object);
        }
    }
    new_tree->setLeft(KDTree::construct_kdtree(left, level + 1, vertexes));
    new_tree->setRight(KDTree::construct_kdtree(right, level + 1, vertexes));
    return new_tree;


}


/*
 * This method recursively traverse the KD Tree and returns true if found any intersection
 * otherwise false
 * instance of Intersection and concrete object returns as a parameter
 */
bool KDTree::get_intersection(const Ray& ray, Triangle **object, Intersection *intersection, vector<cv::Vec3f> &vertexes) {
    bool is_intersect = false;
    float closest_dist = numeric_limits<float>::max();
    float EPS = 0.001;
    for (auto current_object : objects) {
        Intersection actual_intersection = current_object->intersect(ray, vertexes);
        if (actual_intersection.exists) {
            float ray_o_to_i_dist = vec_length(actual_intersection.contact_c_ - ray.origin_c_);
            if (ray_o_to_i_dist > EPS && ray_o_to_i_dist < closest_dist) {
                is_intersect = true;
                closest_dist = ray_o_to_i_dist;
                if (object) {
                    *object = current_object;
                }
                if (intersection) {
                    *intersection = actual_intersection;
                }
            }
        }
    }

    return is_intersect;
}

bool KDTree::traverse_tree(KDTree *node, Ray &ray, Triangle **object, Intersection *intersection, vector<cv::Vec3f> &vertexes) {
        if (isLeave(node) || node->objects.size() <= MIN_OBJECTS) {
        node->get_intersection(ray, object, intersection, vertexes);
        if (intersection->exists) {
            return true;
        }
        return false;
    } else {
        Triangle *objectx, *objecty;
        Intersection intersectionx, intersectiony;
        Intersection int1 = node->getLeft()->getInnerSpaceBox().intersect(ray);
        Intersection int2 = node->getRight()->getInnerSpaceBox().intersect(ray);
        if(!int1.exists && !int2.exists){
            return false;
        }

        float min_dist = MIN_DIST;
        if (int1.exists && KDTree::traverse_tree(node->getLeft(), ray, &objectx, &intersectionx, vertexes)) {
            *object = objectx;
            *intersection = intersectionx;
            min_dist = vec_length(ray.origin_c_ - intersection->contact_c_);
        }
        if (int2.exists && KDTree::traverse_tree(node->getRight(), ray, &objecty, &intersectiony, vertexes)) {
            if(vec_length(intersectiony.contact_c_ - ray.origin_c_) < min_dist){
                *object = objecty;
                *intersection = intersectiony;
            }

        }
        if (intersection->exists) {
            return true;
        }
        return false;
    }
}


Box KDTree::getInnerSpaceBox() {
    return inner_space_box_;
}

KDTree *KDTree::getLeft() const {
    return left_;
}

void KDTree::setLeft(KDTree *left) {
    KDTree::left_ = left;
}

KDTree *KDTree::getRight() const {
    return right_;
}

void KDTree::setRight(KDTree *right) {
    KDTree::right_ = right;
}

double KDTree::getSplit() const {
    return split_;
}

void KDTree::setSplit(double split) {
    KDTree::split_ = split;
}

void KDTree::setLevel(int level) {
    level_ = level;
}



