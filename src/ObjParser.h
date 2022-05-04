//
// Created by Martin Gano on 3.5.2022.
//

#ifndef RAY_TRACER_OBJPARSER_H
#define RAY_TRACER_OBJPARSER_H

#include <iostream>
#include <string>
#include "base.h"
#include "Triangle.h"
#include <vector>

using namespace std;

class ObjParser {
public:
    explicit ObjParser(const char *obj_file_name);

    void parseObjects(vector<Triangle *> &objects, Material mat, vector<cv::Vec3f> &vertexes);

private:
    const char *obj_file_name_{};

};


#endif //RAY_TRACER_OBJPARSER_H
