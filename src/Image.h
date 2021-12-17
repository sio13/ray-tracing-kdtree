//
// Created by Martin Gano on 17.12.2021.
//

#ifndef RAYTRACER_IMAGE_H
#define RAYTRACER_IMAGE_H

#include <opencv2/imgproc/imgproc.hpp>


class Image {
public:
    Image(int width_, int height_);

    ~Image();

    uint32_t &at(int x, int y);

    void set(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);

    cv::Mat get_mat();
    int getWidth() const;
    int getHeight() const;

private:
    uint32_t *buffer;
    int width{};
    int height{};

};


#endif //RAYTRACER_IMAGE_H
