//
// Created by Martin Gano on 3.5.2022.
//

#include "ObjParser.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;

ObjParser::ObjParser(const char *obj_file_name)
        : obj_file_name_(obj_file_name) {
}

void ObjParser::parseObjects(vector<Triangle *> &objects, Material mat, vector<cv::Vec3f> &vertexes) {
    ifstream obj(obj_file_name_, ios_base::in);

    char object_type[50];
    float x_c, y_c, z_c;
    char vertex1[16], vertex2[16], vertex3[16];
    long triangle[3];

    while (obj) {
        obj >> object_type;
        if (strcmp(object_type, "v") == 0) {
            obj >> x_c >> y_c >> z_c;
            vertexes.emplace_back(x_c, y_c, z_c);
        } else if (strcmp(object_type, "f") == 0) {
            obj >> vertex1 >> vertex2 >> vertex3;
            triangle[0] = strtol(vertex1, nullptr, 10);
            if (triangle[0] == 0) {
                cerr << "Conversion was not successful - " << vertex1 << endl;
                exit(1);
            }
            triangle[1] = strtol(vertex2, nullptr, 10);
            if (triangle[1] == 0) {
                cerr << "Conversion was not successful - " << vertex2 << endl;
                exit(1);
            }
            triangle[2] = strtol(vertex3, nullptr, 10);
            if (triangle[2] == 0) {
                cerr << "Conversion was not successful - " << vertex3 << endl;
                exit(1);
            }
            auto *new_primitive = new Triangle(triangle[0] - 1,
                                               triangle[1] - 1,
                                               triangle[2] - 1);
            (*new_primitive).setMaterial(mat);
            objects.push_back(new_primitive);
        }
    }
}


