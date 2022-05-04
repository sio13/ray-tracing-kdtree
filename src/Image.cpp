//
// Created by Martin Gano on 3.5.2022.
//

#include <cstdint>
#include "Image.h"

Image::Image(int width_, int height_) {
    buffer = new uint32_t[width_ * height_];
    width = width_;
    height = height_;
}

Image::~Image() {
    delete[] buffer;
}

uint32_t &Image::at(int x, int y) {
    return buffer[x + y * width];
}

void Image::set(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    uint32_t &pixel = at(x, y);
    auto *components = reinterpret_cast<unsigned char *>(&pixel);
    components[0] = b;
    components[1] = g;
    components[2] = r;
    components[3] = a;
}

cv::Mat Image::get_mat() {
    return {height, width, CV_8UC4, buffer};
}

int Image::getWidth() const {
    return width;
}

int Image::getHeight() const {
    return height;
}

