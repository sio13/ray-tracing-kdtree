//
// Created by Martin Gano on 17.12.2021.
//

#ifndef FINAL_SOLUTION_KDTREE_H
#define FINAL_SOLUTION_KDTREE_H

#include <vector>
#include "base.h"

using namespace std;

class KDTree {
public:
    vector<Primitive *> objects;

    static KDTree *construct_kdtree(vector<Primitive *> objects);

    bool get_intersection(Ray ray, Primitive **object, Intersection *intersection);
};


#endif //FINAL_SOLUTION_KDTREE_H
