//
// Created by Martin Gano on 3.5.2022.
//

#include "Scene.h"

using namespace std;

Ray Ray::reflect(const cv::Vec3f& axis_unit_v) const {
    cv::Vec3f projection = axis_unit_v * direction_unit_v_.dot(axis_unit_v);
    cv::Vec3f step = projection - direction_unit_v_;
    return {origin_c_, direction_unit_v_ + step * 2};
}


Color Scene::rayTracing(Ray ray, vector<cv::Vec3f> &vertexes) {
    Color result(0, 0, 0);

    Triangle *object;
    Material material;
    Intersection intersection;

    bool found;
    if(KDTREE){
        found = KDTree::traverse_tree(kd_tree, ray, &object, &intersection, vertexes);
    } else {
        found = kd_tree->get_intersection(ray, &object, &intersection, vertexes);
    }
    if(!found)
        return result;


    material = object->material_;

    // Calculate ambient
    Color ambient = ambient_color_ * material.ambient_k_;

    // Calculate diffuse and specular
    Color diffuse(0, 0, 0);
    Color specular(0, 0, 0);
    cv::Vec3f reflected = -ray.reflect(intersection.normal_unit_v_).direction_unit_v_;
    for (auto light: lights) {
        Ray light_ray;
        cv::Vec3f light_origin = light->origin_c_;
        light_ray.origin_c_ = light_origin;
        light_ray.direction_unit_v_ = unit_vec(intersection.contact_c_ - light_origin);
        Intersection light_intersection;
        Triangle* object_temp;

        if(KDTREE){
            found =  KDTree::traverse_tree(kd_tree, light_ray, &object_temp, &light_intersection, vertexes);
        } else {
            found = kd_tree->get_intersection(light_ray, nullptr, &light_intersection, vertexes);
        }

        if (!found || !light_intersection.exists
            || vec_length(light_intersection.contact_c_ - intersection.contact_c_) > 0.0001)
            continue;

        float falloff = vec_length(intersection.contact_c_ - light_origin);
        falloff *= falloff;

        float diffusion = intersection.normal_unit_v_.dot(unit_vec(light_origin - intersection.contact_c_));
        if (diffusion > 0)
            diffuse += light->color_ * material.diffuse_f_ * material.diffuse_k_ * light->intensity_ * diffusion *
                       (1 / falloff);

        float specularity = reflected.dot(-light_ray.direction_unit_v_);
        if (specularity > 0)
            specular += light->color_ * material.specular_f_ * material.specular_k_ * light->intensity_ *
                        pow(specularity, material.shininess_) * (1 / falloff);
    }

    result = ambient + diffuse + specular;
    result.clip();
    return result;
}

void Scene::render(Image *image, vector<cv::Vec3f> &vertexes) {
    int width = image->getWidth();
    int height = image->getHeight();
    cv::Vec3f normal = unit_vec(camera->view_plane_point_c - camera->origin_c);
    cv::Vec3f up(0, 0, 1);


    float plane_left_length = camera->view_plane_width / (float) width;
    float plane_up_length = camera->view_plane_height / (float) height;

    cv::Vec3f plane_left = up.cross(unit_vec(normal)) * plane_left_length;
    cv::Vec3f plane_right = -plane_left;
    cv::Vec3f plane_up = normal.cross(unit_vec(plane_left)) * plane_up_length;
    cv::Vec3f plane_down = -plane_up;

    // Coordinate
    cv::Vec3f plane_top_left_corner_coord = camera->view_plane_point_c +
                                      (plane_left * static_cast<float>(width) / 2.0f) +
                                      (plane_up * static_cast<float>(height) / 2.0f);

    // cv::Vec3ftor from camera
    cv::Vec3f plane_top_left_corner = plane_top_left_corner_coord - camera->origin_c;

    for (int w = 0; w < width; ++w) {
        if(VERBOSE){
            cout << "Current raw position is : " << w << endl;
        }

        for (int h = 0; h < height; ++h) {

            Ray ray;
            ray.origin_c_ = camera->origin_c;

            ray.direction_unit_v_ = unit_vec(plane_top_left_corner +
                                     plane_right * static_cast<float>(w) +
                                     plane_down * static_cast<float>(h));


            Color pixel = rayTracing(ray, vertexes);
            pixel.gammaCorrect();
            image->set(w, h, static_cast<unsigned char>(pixel.r() * 255),
                       static_cast<unsigned char>(pixel.g() * 255),
                       static_cast<unsigned char>(pixel.b() * 255),
                       255);
        }
    }
}

void Scene::setAmbientColor(const Color &ambientColor) {
    ambient_color_ = ambientColor;
}