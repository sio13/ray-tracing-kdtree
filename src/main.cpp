//
// Created by Martin Gano on 16.12.2021.
//


#include <vector>
#include <iostream>
#include <opencv2/highgui.hpp>
#include "Scene.h"
#include "KDTree.h"
#include "base.h"
#include "Image.h"
#include "Plane.h"
#include "Triangle.h"
#include "ObjParser.h"
#include "Box.h"
#include "Sphere.h"

using namespace std;

int main(int argc, char **argv) {
    ObjParser obj_parser = ObjParser("data/ladybug_black.obj");
    vector<Primitive *> objects;


    Material mat;
    mat.diffuse_f_ = Color(1, 1, 1);
    mat.reflection_f = Color(1, 1, 1);
    mat.specular_f_ = Color(1, 1, 1);
    //0.01 1.0 0.02 0.3 1.0 50

    mat.ambient_k_ = 0.01;
    mat.diffuse_k_ = 1.0;
    mat.occlusion_k = 0.02;
    mat.reflection_k = 0.3;
    mat.specular_k_ = 1.0;
    mat.shininess_ = 50;
    obj_parser.parseObjects(objects, mat);

//    auto *plane = new Plane(Vec(0, 0, 0), Vec(0, 0, 1));
//    (*plane).setMaterial(mat);
//    objects.push_back(plane);

//    auto *sphere = new Sphere(Vec(1.5, 8, 1), 1);
//    (*sphere).setMaterial(mat);
//    objects.push_back(sphere);

//    auto *box = new Box(Vec(0, 10, 0), Vec(3, 13, 3));
//    (*box).setMaterial(mat);
//    objects.push_back(box);

    Scene scene;
    KDTree *kdtree_root = KDTree::construct_kdtree(objects);
    scene.kd_tree = kdtree_root;

    float plane_height = 240;
    float plane_width = 300;

    scene.camera = new Camera();
    scene.camera->origin_c = Vec(-10, -20, 8);
    scene.camera->view_plane_point_c = Vec(-5, 10, 0);
    scene.camera->view_plane_height = plane_height;
    scene.camera->view_plane_width = plane_width;

    scene.setOcclusionQ(1);
    scene.setSoftlightQ(1);

    scene.lights.push_back(new Light());
    scene.lights[0]->origin_c_ = Vec(5, 0, 10);
    scene.lights[0]->color_ = Color(1, 1, 1);
    scene.lights[0]->intensity_ = 7;
    scene.lights[0]->radius = 0.2;

    scene.setAmbientColor(Color(1, 1, 1));
    scene.setMaxReflectionsCount(1);

    int image_width = 400;
    int image_height = 300;
    Image image(image_width, image_height);
    for (;;) {
        auto start_time = std::chrono::high_resolution_clock::now();
        scene.render(&image);
        auto end_time = std::chrono::high_resolution_clock::now();
        auto time = end_time - start_time;
        std::cout << time / std::chrono::milliseconds(1) << "ms.\n";

        cv::Mat frame = cv::Mat(image_width, image_height, CV_8UC3);
        cv::cvtColor(image.get_mat(), frame, cv::COLOR_BGRA2RGB);

        cv::imshow("Animation", frame);
        int key = cv::waitKey(0);
        std::cout << key;
        if (key == 27) {
            return 0;
        }
        if (key == 3) {
            scene.camera->origin_c = Vec(scene.camera->origin_c.x() + (float)0.5,
                                              scene.camera->origin_c.y(),
                                              scene.camera->origin_c.z());
        }
        if (key == 2) {
            scene.camera->origin_c = Vec(scene.camera->origin_c.x() - (float)0.5,
                                              scene.camera->origin_c.y(),
                                              scene.camera->origin_c.z());
        }
        if (key == 0) {
            scene.camera->origin_c = Vec(scene.camera->origin_c.x(),
                                              scene.camera->origin_c.y(),
                                              scene.camera->origin_c.z() + (float)0.5);
        }
        if (key == 1) {
            scene.camera->origin_c = Vec(scene.camera->origin_c.x(),
                                              scene.camera->origin_c.y(),
                                              scene.camera->origin_c.z() - (float)0.5);
        }
        if (key == 115) {
            scene.camera->origin_c = Vec(scene.camera->origin_c.x(),
                                              scene.camera->origin_c.y() - (float)0.5,
                                              scene.camera->origin_c.z());
        }
        if (key == 119) {
            scene.camera->origin_c = Vec(scene.camera->origin_c.x(),
                                              scene.camera->origin_c.y() + (float)0.5,
                                              scene.camera->origin_c.z());
        }

    }

}