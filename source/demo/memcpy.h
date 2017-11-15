#pragma once

#include <core/bench.h>
#include <core/exporter.h>
#include <core/defines.h>

#include <cstring>
#include <fstream>
#include <algorithm>
#include <random>

void* memcpy_parallel(void* destination, const void* source, size_t num);
void* memcpy_vector(void* destination, const void* source, size_t num);

void run_memory_benchmark(std::ofstream& stream)
{
    // TODO: Set random data each run.
    std::vector<uint8_t> dst(256 * 1024 * 1024);
    std::vector<uint8_t> src(256 * 1024 * 1024);
    std::vector<int> sample_sizes = {KB, 32 * KB, 64 * KB, 128 * KB, 256 * KB, 512 * KB,
                                     MB, 32 * MB, 64 * MB, 128 * MB, 256 * MB};
    auto randomise = [&](int sample) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, 255);

        for (int i = 0; i < sample; ++i) {
            src[i] = static_cast<uint8_t>(dis(gen));
        }
    };

    std::for_each(std::begin(sample_sizes), std::end(sample_sizes), [&](auto&& sample) {
        randomise(sample);
        auto normal = bench::run<double, std::milli>([&] { std::memcpy(dst.data(), src.data(), sample); });
        randomise(sample);
        auto vector = bench::run<double, std::milli>([&] { memcpy_vector(dst.data(), src.data(), sample); });
        randomise(sample);
        auto parallel = bench::run<double, std::milli>([&] { memcpy_parallel(dst.data(), src.data(), sample); });
        stream << exporter::execute<double, std::milli>(sample, {"normal", "vector", "parallel"},
                                                        {normal, vector, parallel});
    });
}
