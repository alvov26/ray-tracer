//
// Created by Aleksandr Lvov on 2021-11-02.
//

#include "Vec3.h"

#include <iostream>

std::ostream &operator<<(std::ostream &os, const Vec3 &v) {
    return os << "Vec3(x: " << v.x()
              << ", y: " << v.y()
              << ", z: " << v.z() << ")";
}
