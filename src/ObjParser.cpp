//
// Created by Martin Gano on 18.12.2021.
//

#include "ObjParser.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;

ObjParser::ObjParser(const char *obj_file_name)
        : obj_file_name_(obj_file_name) {
}

void ObjParser::parseObjects(vector<Primitive *> &objects, Material mat) {
    ifstream obj(obj_file_name_, ios_base::in);

    char object_type[50];
    float x_c, y_c, z_c;
    char vertex1[16], vertex2[16], vertex3[16];
    long triangle[3];

    vector<Vec> vertexes;

    while (obj) {
        obj >> object_type;
        if (strcmp(object_type, "v") == 0) {
            obj >> x_c >> y_c >> z_c;
            vertexes.emplace_back(x_c, y_c, z_c);
        } else if (strcmp(object_type, "f") == 0) {
            obj >> vertex1 >> vertex2 >> vertex3;

            triangle[0] = strtol(vertex1, nullptr, 10);
            if (triangle[0] == 0) {
                cerr << "Conversion was not successful" << endl;
            }
            triangle[1] = strtol(vertex2, nullptr, 10);
            if (triangle[1] == 0) {
                cerr << "Conversion was not successful" << endl;
            }
            triangle[2] = strtol(vertex3, nullptr, 10);
            if (triangle[2] == 0) {
                cerr << "Conversion was not successful" << endl;
            }
            auto *new_primitive = new Triangle(vertexes[triangle[0] - 1],
                                          vertexes[triangle[1] - 1],
                                          vertexes[triangle[2] - 1]);
            (*new_primitive).setMaterial(mat);
            objects.push_back(new_primitive);
        }
    }
}


