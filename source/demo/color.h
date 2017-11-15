#pragma once

#include <core/bench.h>
#include <core/exporter.h>
#include <core/defines.h>

#include <cstring>
#include <fstream>
#include <algorithm>
#include <memory>
#include <random>

std::unique_ptr<float[]> yuv_to_rgba(const float* yuv, int size);
std::unique_ptr<float[]> yuv_to_rgba_vector(const float* yuv, int size);
std::unique_ptr<float[]> yuv_to_rgba_parallel(const float* yuv, int size);
std::unique_ptr<float[]> yuv_to_rgba_parallel_vector(const float* yuv, int size);

void run_color_benchmark(std::ofstream& stream)
{
    // TODO: Set random data each run.
    std::vector<float> src(3 * 3840 * 2160);
    std::vector<int> sample_sizes = {3 * 720 * 480,   3 * 720 * 576,   3 * 1280 * 720, 3 * 1920 * 1080,
                                     3 * 1920 * 1200, 3 * 2160 * 1440, 3 * 3840 * 2160};
    auto randomise = [&](int sample) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(.0f, 1.f);

        for (int i = 0; i < sample; ++i) {
            src[i] = dis(gen);
        }
    };

    std::for_each(std::begin(sample_sizes), std::end(sample_sizes), [&](auto&& sample) {
        randomise(sample);
        auto normal = bench::run<double, std::milli>([&] { yuv_to_rgba(src.data(), sample); });
        randomise(sample);
        auto vector = bench::run<double, std::milli>([&] { yuv_to_rgba_vector(src.data(), sample); });
        randomise(sample);
        auto parallel = bench::run<double, std::milli>([&] { yuv_to_rgba_parallel(src.data(), sample); });
        randomise(sample);
        auto parallel_vector = bench::run<double, std::milli>([&] { yuv_to_rgba_parallel_vector(src.data(), sample); });
        stream << exporter::execute<double, std::milli>(sample, {"normal", "vector", "parallel", "parallel_vector"},
                                                        {normal, vector, parallel, parallel_vector});
    });
}
