//
// Created by Martin Gano on 17.12.2021.
//

#include "Scene.h"

using namespace std;

Ray Ray::reflect(Vec axis_unit_v) const {
    Vec projection = axis_unit_v * direction_unit_v_.dot(axis_unit_v);
    Vec step = projection - direction_unit_v_;
    return {origin_c_, direction_unit_v_ + step * 2};
}

Vec randomUnitVector() {
    const auto rand_max = static_cast<float>(RAND_MAX);
    return Vec((static_cast<float>(rand()) / rand_max) - 0.5f,
               (static_cast<float>(rand()) / rand_max) - 0.5f,
               (static_cast<float>(rand()) / rand_max) - 0.5f).unit();
}

Color Scene::traceRay(Ray ray, int reflection_count) {
    Color result(0, 0, 0);
    if (reflection_count > max_reflections_count_)
        return result;

    Primitive *object;
    Material material;
    Intersection intersection;

    if (!kd_tree->get_intersection(ray, &object, &intersection))
        return result;


    material = object->material_;

    // Calculate ambient
    Color ambient = ambient_color_ * material.ambient_k_;

    // Calculate diffuse and specular
    Color diffuse(0, 0, 0);
    Color specular(0, 0, 0);
    Vec reflected = -ray.reflect(intersection.normal_unit_v_).direction_unit_v_;
    for (auto light : lights) {
        int softlight_steps = softlight_q > 0 ? softlight_q : 1;
        for (int j = 0; j < softlight_steps; ++j) {
            Ray light_ray;
            Vec light_origin = light->origin_c_;
            if (softlight_q > 0)
                light_origin += randomUnitVector() * light->radius;
            light_ray.origin_c_ = light_origin;
            light_ray.direction_unit_v_ = (intersection.contact_c_ - light_origin).unit();
            Intersection light_intersection;
            bool found = kd_tree->get_intersection(light_ray, nullptr, &light_intersection);

            if (!found || !light_intersection.exists
                || (light_intersection.contact_c_ - intersection.contact_c_).length() > 0.0001)
                continue;

            float falloff = (intersection.contact_c_ - light_origin).length();
            falloff *= falloff;

            float diffusion = intersection.normal_unit_v_.dot((light_origin - intersection.contact_c_).unit());
            if (diffusion > 0)
                diffuse += light->color_ * material.diffuse_f_ * material.diffuse_k_ * light->intensity_ * diffusion *
                           (1 / falloff);

            float specularity = reflected.dot(-light_ray.direction_unit_v_);
            if (specularity > 0)
                specular += light->color_ * material.specular_f_ * material.specular_k_ * light->intensity_ *
                            pow(specularity, material.shininess_) * (1 / falloff);
        }
        diffuse = diffuse / static_cast<float>(softlight_steps);
        specular = specular / static_cast<float>(softlight_steps);
    }

    // Calculate ambient occlusion
    Color occlusion(0, 0, 0);
    if (occlusion_q_) {
        float occlusion_amount = 1.0f;
        const float occlusion_step = 1.0f / (float) occlusion_q_;
        for (int i = 0; i < occlusion_q_; ++i) {
            Vec random_dir = randomUnitVector();
            random_dir = random_dir.unit();
            if (random_dir.dot(intersection.normal_unit_v_) < 0)
                random_dir = -random_dir;
            Ray occlusion_ray(intersection.contact_c_, random_dir);
            Primitive *occlusion_object;
            Intersection occlusion_intersection;
            if (kd_tree->get_intersection(occlusion_ray, &occlusion_object, &occlusion_intersection)) {
                occlusion_amount -= occlusion_step;
            }
        }
        occlusion = material.diffuse_f_ * occlusion_amount * material.occlusion_k;
    }

    // Calculate reflection
    Color reflection = traceRay(Ray(intersection.contact_c_, reflected.unit()), reflection_count + 1)
                       * material.reflection_f
                       * material.reflection_k;

    result = ambient + diffuse + specular + occlusion + reflection;
    result.clip();
    return result;
}

void Scene::render(Image *image) {
    int width = image->getWidth();
    int height = image->getHeight();
    Vec normal = (camera->view_plane_point_c - camera->origin_c).unit();
    Vec up(0, 0, 1);


    float plane_left_length = camera->view_plane_width / (float)width;
    float plane_up_length = camera->view_plane_height / (float)height;

    Vec plane_left = up.cross(normal).unit() * plane_left_length;
    Vec plane_right = -plane_left;
    Vec plane_up = normal.cross(plane_left).unit() * plane_up_length;
    Vec plane_down = -plane_up;

    // Coordinate
    Vec plane_top_left_corner_coord = camera->view_plane_point_c +
                                     (plane_left * static_cast<float>(width) / 2.0f) +
                                     (plane_up * static_cast<float>(height) / 2.0f);

    // Vector from camera
    Vec plane_top_left_corner = plane_top_left_corner_coord - camera->origin_c;

    for (int w = 0; w < width; ++w) {
        cout << w << endl;
        for (int h = 0; h < height; ++h) {

            Ray ray;
            ray.origin_c_ = camera->origin_c;

            ray.direction_unit_v_ = (plane_top_left_corner +
                                       plane_right * static_cast<float>(w) +
                                       plane_down * static_cast<float>(h)).unit();


            Color pixel = traceRay(ray);
            pixel.gammaCorrect();
            image->set(w, h, static_cast<unsigned char>(pixel.r() * 255),
                       static_cast<unsigned char>(pixel.g() * 255),
                       static_cast<unsigned char>(pixel.b() * 255),
                       255);
        }
    }
}


void Scene::setSoftlightQ(int softlightQ) {
    softlight_q = softlightQ;
}

void Scene::setOcclusionQ(int occlusionQ) {
    occlusion_q_ = occlusionQ;
}

void Scene::setAmbientColor(const Color &ambientColor) {
    ambient_color_ = ambientColor;
}

void Scene::setMaxReflectionsCount(int maxReflectionsCount) {
    max_reflections_count_ = maxReflectionsCount;
}
