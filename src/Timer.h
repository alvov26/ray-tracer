//
// Created by Aleksandr Lvov on 2021-11-04.
//

#pragma once

#include <chrono>

using std::chrono::system_clock;
class Timer {
    decltype(system_clock::now()) start = system_clock::now();

public:
    auto seconds() const {
        auto end = system_clock::now();
        return std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    }
};
