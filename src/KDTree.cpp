//
// Created by Martin Gano on 17.12.2021.
//

#include "KDTree.h"

#include "Triangle.h"
#include <iostream>
#include <utility>

using namespace std;


//KDTree *KDTree::construct_kdtree(vector<Primitive *> objects) {
//    auto *kdtree = new KDTree();
//    kdtree->objects = move(objects);
//    return kdtree;
//}

KDTree::KDTree(const vector<Triangle *> &objects, Box innerSpaceBox, int axis, KDTree *left, KDTree *right,
               double split) : objects(objects), inner_space_box_(std::move(innerSpaceBox)), axis_(axis), left_(left),
                               right_(right), split_(split) {}


KDTree *KDTree::construct_kdtree(vector<Triangle *> &objects, unsigned int &max_objects, int level) {

    Vec max_bound = Vec(objects[0]->getP0().x(), objects[0]->getP0().y(), objects[0]->getP0().z());
    Vec min_bound = Vec(objects[0]->getP0().x(), objects[0]->getP0().y(), objects[0]->getP0().z());

    for (auto &object : objects) {
        if (getMin(0, *object) < min_bound.x()) {
            min_bound.setX(getMin(0, *object));
        }
        if (getMin(1, *object) < min_bound.y()) {
            min_bound.setY(getMin(1, *object));
        }
        if (getMin(2, *object) < min_bound.z()) {
            min_bound.setZ(getMin(2, *object));
        }

        if (getMax(0, *object) > max_bound.x()) {
            max_bound.setX(getMax(0, *object));
        }
        if (getMax(1, *object) > max_bound.y()) {
            max_bound.setY(getMax(1, *object));
        }
        if (getMax(2, *object) > max_bound.x()) {
            max_bound.setZ(getMax(2, *object));
        }
    }

    Box inner_bound_box = Box(min_bound, max_bound);
    auto *new_tree = new KDTree(objects, inner_bound_box, level % 3, nullptr, nullptr, 0);


    if (level >= 20 || objects.size() <= max_objects) {
        new_tree->setRight(nullptr);
        new_tree->setLeft(nullptr);
        return new_tree;
    }

    double ideal_split = getMax(new_tree->axis_, *objects[objects.size() / 2]);
    new_tree->setSplit(ideal_split);

    vector<Triangle *> left;
    vector<Triangle *> right;

    for (auto & object : objects) {
        if (getMax(new_tree->axis_, *object) < new_tree->getSplit()) {
            left.push_back(object);
        } else if (getMin(new_tree->axis_, *object) > new_tree->getSplit()) {
            right.push_back(object);
        } else {
            left.push_back(object);
            right.push_back(object);
        }
    }
    new_tree->setLeft(KDTree::construct_kdtree(left, max_objects, level + 1));
    new_tree->setRight(KDTree::construct_kdtree(right, max_objects, level + 1));

    return new_tree;


}


/*
 * This method recursively traverse the KD Tree and returns true if found any intersection
 * otherwise false
 * instance of Intersection and concrete object returns as a parameter
 */
bool KDTree::get_intersection(Ray ray, Triangle **object, Intersection *intersection) {
    bool is_intersect = false;
    float closest_dist = std::numeric_limits<float>::max();
    float EPS = 0.001;
    for (auto current_object : objects) {
        Intersection actual_intersection = current_object->intersect(ray);
        if (actual_intersection.exists) {
            float ray_o_to_i_dist = (actual_intersection.contact_c_ - ray.origin_c_).length();
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


bool isLeave(KDTree *node){
    return node->getLeft() == nullptr && node->getRight() == nullptr;
}

Intersection KDTree::traverse_tree(KDTree *node, Ray &ray){
    if(!isLeave(node)){
        KDTree *left = node->getLeft();
        left->getInnerSpaceBox().intersect(ray);
    }
}



Box KDTree::getInnerSpaceBox() {
    return inner_space_box_;
}

void KDTree::setInnerSpaceBox(const Box &innerSpaceBox) {
    inner_space_box_ = innerSpaceBox;
}

int KDTree::getAxis() const {
    return axis_;
}

void KDTree::setAxis(int axis) {
    KDTree::axis_ = axis;
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



