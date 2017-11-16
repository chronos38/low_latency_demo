#pragma once

#include <core/bench.h>
#include <core/exporter.h>
#include <core/defines.h>

#include <cstring>
#include <fstream>
#include <algorithm>
#include <random>
#include <memory>

void* memcpy_parallel(void* destination, const void* source, size_t num);
void* memcpy_vector(void* destination, const void* source, size_t num);

void run_memory_benchmark(std::ofstream& stream)
{
    auto src = std::make_unique<uint8_t[]>(1024 * 1024 * 1024);
    auto dst = std::make_unique<uint8_t[]>(1024 * 1024 * 1024);
    std::vector<int> sample_sizes = {KB,     8 * KB,  16 * KB, 32 * KB, 64 * KB,  128 * KB, 256 * KB, 512 * KB, MB,
                                     8 * MB, 16 * MB, 32 * MB, 64 * MB, 128 * MB, 256 * MB, 512 * MB, GB};
    auto randomise = [&](int sample) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, 255);

        for (int i = 0; i < sample; ++i) {
            src[i] = static_cast<uint8_t>(dis(gen));
        }
    };

    stream << "Bytes" << exporter::separator << "Type" << exporter::separator << "Time (ms)" << std::endl;
    std::for_each(std::begin(sample_sizes), std::end(sample_sizes), [&](auto&& sample) {
        auto normal =
            benchmark_runner([&] { randomise(sample); }, [&] { std::memcpy(dst.get(), src.get(), sample); }, [] {});
        auto vector =
            benchmark_runner([&] { randomise(sample); }, [&] { memcpy_vector(dst.get(), src.get(), sample); }, [] {});
        auto parallel =
            benchmark_runner([&] { randomise(sample); }, [&] { memcpy_parallel(dst.get(), src.get(), sample); }, [] {});
        stream << exporter::execute<benchmark_rep, benchmark_period>(sample, {"normal", "vector", "parallel"},
                                                                     {normal, vector, parallel});
    });
}
