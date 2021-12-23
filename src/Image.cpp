//
// Created by Aleksandr Lvov on 2021-11-01.
//

#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stbi_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Image::Image(size_t w, size_t h)
    : width_(w), height_(h), data_(w * h, Pixel::fromRGB(0, 0, 0)) {}

Image::Image(size_t w, size_t h, std::vector<Pixel> buffer)
    : width_(w), height_(h), data_(std::move(buffer)) {}

void Image::writeToFile(std::string_view filename) {
    stbi_write_jpg(filename.data(), static_cast<int>(width_), static_cast<int>(height_), 4, data_.data(), 100);
}

Pixel &Image::at(size_t x, size_t y) {
    return data_[x + y * width_];
}

const Pixel &Image::at(size_t x, size_t y) const {
    return data_[x + y * width_];
}

Image Image::readFromFile(std::string_view filename) {
    int width, height, n;
    Pixel *data = reinterpret_cast<Pixel *>(stbi_load(filename.data(), &width, &height, &n, 4));
    if (!data)
        throw std::runtime_error("Could not read image from disk");
    std::vector<Pixel> buffer(data, data + width * height);
    stbi_image_free(data);
    return Image{
            static_cast<size_t>(width),
            static_cast<size_t>(height),
            std::move(buffer)};
}
