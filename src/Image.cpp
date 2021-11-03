//
// Created by Aleksandr Lvov on 2021-11-01.
//

#include "Image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Image::Image(size_t w, size_t h) : width_(w), height_(h), data_(w * h, Pixel::fromRGB(0, 0, 0)) {}

void Image::writeToFile(std::string_view filename) {
    stbi_write_jpg(filename.data(), static_cast<int>(width_), static_cast<int>(height_), 4, data_.data(), 100);
}

Pixel &Image::at(size_t x, size_t y) {
    return data_[x + y * width_];
}
