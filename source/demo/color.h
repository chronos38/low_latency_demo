#pragma once

#include <core/bench.h>
#include <core/exporter.h>
#include <core/defines.h>

#include <cstring>
#include <fstream>
#include <algorithm>
#include <memory>
#include <random>

float* yuv_to_rgba(const float* yuv, int size);
float* yuv_to_rgba_vector(const float* yuv, int size);
float* yuv_to_rgba_parallel(const float* yuv, int size);

void run_color_benchmark(std::ofstream& stream)
{
    std::vector<float> src(3 * 3840 * 2160);
    std::vector<int> sample_sizes = {3 * 620 * 480,   3 * 720 * 480,   3 * 720 * 576,   3 * 1024 * 768,
                                     3 * 1280 * 720,  3 * 1280 * 1024, 3 * 1920 * 1080, 3 * 1920 * 1200,
                                     3 * 2160 * 1440, 3 * 3840 * 2160};
    auto randomise = [&](int sample) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(.0f, 1.f);

        for (int i = 0; i < sample; ++i) {
            src[i] = dis(gen);
        }
    };

    stream << "Bytes" << exporter::separator << "Type" << exporter::separator << "Time (ms)" << std::endl;
    std::for_each(std::begin(sample_sizes), std::end(sample_sizes), [&](auto&& sample) {
        auto normal =
            benchmark_runner([&] { randomise(sample); }, [&] { delete[] yuv_to_rgba(src.data(), sample); }, [] {});
        auto vector = benchmark_runner([&] { randomise(sample); },
                                       [&] { delete[] yuv_to_rgba_vector(src.data(), sample); }, [] {});
        auto parallel = benchmark_runner([&] { randomise(sample); },
                                         [&] { delete[] yuv_to_rgba_parallel(src.data(), sample); }, [] {});
        stream << exporter::execute<benchmark_rep, benchmark_period>(sample, {"normal", "vector", "parallel"},
                                                                     {normal, vector, parallel});
    });
}
