//
// Created by Aleksandr Lvov on 2021-11-04.
//

#pragma once

#include <chrono>

class Timer {
    using time = std::chrono::time_point<std::chrono::system_clock>;
    time start = std::chrono::system_clock::now();

public:
    [[nodiscard]]
    auto elapsed() const {
        using namespace std::chrono;
        auto end = system_clock::now();
        return duration_cast<seconds>(end - start).count();
    }
};
