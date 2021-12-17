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
#include "Box.h"

using namespace std;

int main(int argc, char **argv) {
    vector<Primitive *> objects;
    Material mat;
    mat.diffuse_f_ = Color(1, 1, 1);
    mat.reflection_f = Color(1, 1, 1);
    mat.specular_f_ = Color(1, 1, 1);
    //0.01 1.0 0.02 0.3 1.0 50

    mat.ambient_k_ = 0.01;
    mat.diffuse_k_ = 1.0;
    mat.occlusion_k = 0.02;
    mat.reflection_k = 0;
    mat.specular_k_ = 1.0;
    mat.shininess_ = 50;
    auto *plane = new Plane(Vec(0, 0, 0), Vec(0, 0, 1));
    (*plane).setMaterial(mat);
    objects.push_back(plane);

//    auto *triangle = new Triangle(Vec(0.8, 14, 3), Vec(1.5, 13, 0), Vec(3, 14, 1.5));
//    (*triangle).setMaterial(mat);
//    objects.push_back(triangle);

    auto *box = new Box(Vec(0, 10, 0), Vec(3, 13, 3));
    (*box).setMaterial(mat);
    objects.push_back(box);

    Scene scene;
    KDTree *kdtree_root = KDTree::construct_kdtree(objects);
    scene.kd_tree = kdtree_root;

    float plane_height = 12;
    float plane_width = 18;

    scene.camera = new Camera();
    scene.camera->origin_c = Vec(8, 0, 8);
    scene.camera->view_plane_point_c = Vec(0, 16, 0);
    scene.camera->view_plane_height = plane_height;
    scene.camera->view_plane_width = plane_width;

    scene.setOcclusionQ(20);
    scene.setSoftlightQ(6);

    scene.lights.push_back(new Light());
    scene.lights[0]->origin_c_ = Vec(8, 0, 16);
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

    }

}