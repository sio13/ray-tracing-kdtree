//
// Created by Martin Gano on 18.12.2021.
//

#ifndef RAYTRACER_OBJPARSER_H
#define RAYTRACER_OBJPARSER_H

#include <iostream>
#include <string>
#include "base.h"
#include "Triangle.h"
#include <vector>
#include "Vec.h"

using namespace std;

class ObjParser {
public:
    explicit ObjParser(const char *obj_file_name);

    void parseObjects(vector<Primitive *> &objects, Material mat);

private:
    const char *obj_file_name_{};

};


#endif //RAYTRACER_OBJPARSER_H
