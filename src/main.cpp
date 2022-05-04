//
// Created by Martin Gano on 3.5.2022.
//


#include <vector>
#include <iostream>
#include <chrono>
#include <opencv2/highgui.hpp>
#include <opencv2/core/matx.hpp>
#include <opencv2/core/mat.hpp>
#include "Scene.h"
#include <cmath>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "KDTree.h"
#include "base.h"
#include "Image.h"
#include "Triangle.h"
#include "ObjParser.h"

using namespace std;
using namespace cv;

unsigned long long Triangle::total_intersections = 0;


int main(int argc, char **argv) {
    ObjParser obj_parser = ObjParser("demo_data/bunny.obj");
    vector<Triangle *> objects;

    Material mat;
    mat.diffuse_f_ = Color(1, 1, 1);
    mat.specular_f_ = Color(1, 1, 1);

    mat.ambient_k_ = 0.01;
    mat.diffuse_k_ = 1.0;
    mat.specular_k_ = 1.0;
    mat.shininess_ = 50;
    vector<cv::Vec3f> vertexes;
    obj_parser.parseObjects(objects, mat, vertexes);

    Scene scene;

    auto start = chrono::steady_clock::now();
    KDTree *kdtree_root = KDTree::construct_kdtree(objects, 0, vertexes);
    auto end = chrono::steady_clock::now();
    cout << "KD Tree construction took: "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count()
         << " ms"
         << endl;

    scene.kd_tree = kdtree_root;

    float plane_height = HEIGHT;
    float plane_width = WIDTH;

    scene.camera = new Camera();
    scene.camera->origin_c = cv::Vec3f(0, -120, 0);
    scene.camera->view_plane_point_c = cv::Vec3f(0, 0, 0);
    scene.camera->view_plane_height = plane_height;
    scene.camera->view_plane_width = plane_width;

    scene.lights.push_back(new Light());
    scene.lights[0]->origin_c_ = cv::Vec3f(0, -120, 0);
    scene.lights[0]->color_ = Color(1, 1, 1);
    scene.lights[0]->intensity_ = INTENSITY;


    scene.setAmbientColor(Color(1, 1, 1));

    int image_width = WIDTH;
    int image_height = HEIGHT;
    Image image(image_width, image_height);
    for (;;) {
        auto start_time = std::chrono::high_resolution_clock::now();
        scene.render(&image, vertexes);
        cout << "Total number of required intersections: " << Triangle::total_intersections << endl;
        auto end_time = std::chrono::high_resolution_clock::now();
        auto time = end_time - start_time;
        std::cout << time / std::chrono::milliseconds(1) << "ms.\n";

        cv::Mat frame = cv::Mat(image_width, image_height, CV_8UC3);
        cv::cvtColor(image.get_mat(), frame, cv::COLOR_BGRA2RGB);

        cv::imshow("Demo App", frame);
        int key = cv::waitKey(0);
        std::cout << key << endl;
        if (key == 27) {
            return 0;
        }
        if (key == 3) {
            cv::Mat_<float> rotation_matrix = cv::Mat::eye(4, 4, CV_32FC1);
            rotation_matrix(0, 0) = 1.1;
            rotation_matrix(1, 1) = 1.1;
            rotation_matrix(2, 2) = 1.1;
            for (auto & vertexe : vertexes) {
                Mat_<float> pm(4,1);
                pm << vertexe[0], vertexe[1], vertexe[2], 1.0;

                Mat_<float> pr =  rotation_matrix * pm;
                vertexe = Vec3f(pr(0), pr(1),pr(2));
            }
            start = chrono::steady_clock::now();
            kdtree_root = KDTree::construct_kdtree(objects, 0, vertexes);
            end = chrono::steady_clock::now();
            cout << "KD Tree construction took: "
                 << chrono::duration_cast<chrono::milliseconds>(end - start).count()
                 << " ms"
                 << endl;
            scene.kd_tree = kdtree_root;

        }
        if (key == 2) {
            cv::Mat_<float> rotation_matrix = cv::Mat::eye(4, 4, CV_32FC1);
            rotation_matrix(0, 0) = 0.9;
            rotation_matrix(1, 1) = 0.9;
            rotation_matrix(2, 2) = 0.9;
            for (auto & vertexe : vertexes) {
                Mat_<float> pm(4,1);
                pm << vertexe[0], vertexe[1], vertexe[2], 1.0;

                Mat_<float> pr =  rotation_matrix * pm;
                vertexe = Vec3f(pr(0), pr(1),pr(2));
            }
            start = chrono::steady_clock::now();
            kdtree_root = KDTree::construct_kdtree(objects, 0, vertexes);
            end = chrono::steady_clock::now();
            cout << "KD Tree construction took: "
                 << chrono::duration_cast<chrono::milliseconds>(end - start).count()
                 << " ms"
                 << endl;
            scene.kd_tree = kdtree_root;
        }
        if (key == 97) {
            cv::Mat_<float> rotation_matrix = cv::Mat::eye(4, 4, CV_32FC1);
            rotation_matrix(1, 1) = 0.98480775301;
            rotation_matrix(1, 2) = 0.17364817766;
            rotation_matrix(2, 1) = -0.17364817766;
            rotation_matrix(2, 2) = 0.98480775301;
            for (auto & vertexe : vertexes) {
                Mat_<float> pm(4,1);
                pm << vertexe[0], vertexe[1], vertexe[2], 1.0;

                Mat_<float> pr =  rotation_matrix * pm;
                vertexe = Vec3f(pr(0), pr(1),pr(2));
            }
            start = chrono::steady_clock::now();
            kdtree_root = KDTree::construct_kdtree(objects, 0, vertexes);
            end = chrono::steady_clock::now();
            cout << "KD Tree construction took: "
                 << chrono::duration_cast<chrono::milliseconds>(end - start).count()
                 << " ms"
                 << endl;
            scene.kd_tree = kdtree_root;
        }
        if (key == 115) {
            cv::Mat_<float> rotation_matrix = cv::Mat::eye(4, 4, CV_32FC1);
            rotation_matrix(1, 1) = 0.98480775301;
            rotation_matrix(1, 2) = -0.17364817766;
            rotation_matrix(2, 1) = 0.17364817766;
            rotation_matrix(2, 2) = 0.98480775301;
            for (auto & vertexe : vertexes) {
                Mat_<float> pm(4,1);
                pm << vertexe[0], vertexe[1], vertexe[2], 1.0;

                Mat_<float> pr =  rotation_matrix * pm;
                vertexe = Vec3f(pr(0), pr(1),pr(2));
            }
            start = chrono::steady_clock::now();
            kdtree_root = KDTree::construct_kdtree(objects, 0, vertexes);
            end = chrono::steady_clock::now();
            cout << "KD Tree construction took: "
                 << chrono::duration_cast<chrono::milliseconds>(end - start).count()
                 << " ms"
                 << endl;
            scene.kd_tree = kdtree_root;
        }

        if (key == 100) {
            cv::Mat_<float> rotation_matrix = cv::Mat::eye(4, 4, CV_32FC1);
            rotation_matrix(0, 0) = 0.98480775301;
            rotation_matrix(0, 2) = -0.17364817766;
            rotation_matrix(2, 0) = 0.17364817766;
            rotation_matrix(2, 2) = 0.98480775301;
            for (auto & vertexe : vertexes) {
                Mat_<float> pm(4,1);
                pm << vertexe[0], vertexe[1], vertexe[2], 1.0;

                Mat_<float> pr =  rotation_matrix * pm;
                vertexe = Vec3f(pr(0), pr(1),pr(2));
            }
            start = chrono::steady_clock::now();
            kdtree_root = KDTree::construct_kdtree(objects, 0, vertexes);
            end = chrono::steady_clock::now();
            cout << "KD Tree construction took: "
                 << chrono::duration_cast<chrono::milliseconds>(end - start).count()
                 << " ms"
                 << endl;
            scene.kd_tree = kdtree_root;
        }
        if (key == 102) {
            cv::Mat_<float> rotation_matrix = cv::Mat::eye(4, 4, CV_32FC1);
            rotation_matrix(0, 0) = 0.98480775301;
            rotation_matrix(0, 2) = 0.17364817766;
            rotation_matrix(2, 0) = -0.17364817766;
            rotation_matrix(2, 2) = 0.98480775301;
            for (auto & vertexe : vertexes) {
                Mat_<float> pm(4,1);
                pm << vertexe[0], vertexe[1], vertexe[2], 1.0;

                Mat_<float> pr =  rotation_matrix * pm;
                vertexe = Vec3f(pr(0), pr(1),pr(2));
            }
            start = chrono::steady_clock::now();
            kdtree_root = KDTree::construct_kdtree(objects, 0, vertexes);
            end = chrono::steady_clock::now();
            cout << "KD Tree construction took: "
                 << chrono::duration_cast<chrono::milliseconds>(end - start).count()
                 << " ms"
                 << endl;
            scene.kd_tree = kdtree_root;
        }

        if (key == 103) {
            cv::Mat_<float> rotation_matrix = cv::Mat::eye(4, 4, CV_32FC1);
            rotation_matrix(0, 0) = 0.98480775301;
            rotation_matrix(0, 1) = -0.17364817766;
            rotation_matrix(1, 0) = 0.17364817766;
            rotation_matrix(1, 1) = 0.98480775301;
            for (auto & vertexe : vertexes) {
                Mat_<float> pm(4,1);
                pm << vertexe[0], vertexe[1], vertexe[2], 1.0;

                Mat_<float> pr =  rotation_matrix * pm;
                vertexe = Vec3f(pr(0), pr(1),pr(2));
            }
            start = chrono::steady_clock::now();
            kdtree_root = KDTree::construct_kdtree(objects, 0, vertexes);
            end = chrono::steady_clock::now();
            cout << "KD Tree construction took: "
                 << chrono::duration_cast<chrono::milliseconds>(end - start).count()
                 << " ms"
                 << endl;
            scene.kd_tree = kdtree_root;
        }
        if (key == 104) {
            cv::Mat_<float> rotation_matrix = cv::Mat::eye(4, 4, CV_32FC1);
            rotation_matrix(0, 0) = 0.98480775301;
            rotation_matrix(0, 1) = 0.17364817766;
            rotation_matrix(1, 0) = -0.17364817766;
            rotation_matrix(1, 1) = 0.98480775301;
            for (auto & vertexe : vertexes) {
                Mat_<float> pm(4,1);
                pm << vertexe[0], vertexe[1], vertexe[2], 1.0;

                Mat_<float> pr =  rotation_matrix * pm;
                vertexe = Vec3f(pr(0), pr(1),pr(2));
            }
            start = chrono::steady_clock::now();
            kdtree_root = KDTree::construct_kdtree(objects, 0, vertexes);
            end = chrono::steady_clock::now();
            cout << "KD Tree construction took: "
                 << chrono::duration_cast<chrono::milliseconds>(end - start).count()
                 << " ms"
                 << endl;
            scene.kd_tree = kdtree_root;
        }

    }

}
