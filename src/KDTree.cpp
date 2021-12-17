//
// Created by Martin Gano on 17.12.2021.
//

#include "KDTree.h"

#include <utility>
#include <iostream>

using namespace std;


KDTree *KDTree::construct_kdtree(vector<Primitive *> objects) {
    auto *kdtree = new KDTree();
    kdtree->objects = move(objects);
    return kdtree;
}


/*
 * This method recursively traverse the KD Tree and returns true if found any intersection
 * otherwise false
 * instance of Intersection and concrete object returns as a parameter
 */
bool KDTree::get_intersection(Ray ray, Primitive **object, Intersection *intersection) {
    bool found = false;
    float distance = -1;

    for (auto current_object : objects) {
        Intersection current_intersection = current_object->intersect(ray);

        if (current_intersection.exists) {
            float current_distance = (current_intersection.contact_c_ - ray.origin_c_).length();
            if (current_distance > 0.001 && (distance < 0 || current_distance < distance)) {
                found = true;
                distance = current_distance;
                if (object){
                    *object = current_object;
                }
                if (intersection)
                    *intersection = current_intersection;
            }
        }
    }

    return found;
}