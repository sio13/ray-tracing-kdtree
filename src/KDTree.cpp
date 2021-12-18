//
// Created by Martin Gano on 17.12.2021.
//

#include "KDTree.h"

#include <utility>

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