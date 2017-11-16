#pragma once

#include <chrono>
#include <functional>

#define KB (1024)
#define MB (1024 * KB)
#define GB (1024 * MB)

typedef std::chrono::duration<double, std::milli> benchmark_result;
typedef benchmark_result::rep benchmark_rep;
typedef benchmark_result::period benchmark_period;

static const auto benchmark_runner = [](const std::function<void()>& begin, const std::function<void()>& cb,
                                        const std::function<void()>& end) {
    const size_t runs = 5;
    benchmark_result result = benchmark_result(0);

    for (size_t i = 0; i < runs; ++i) {
        begin();
        result += bench::run<benchmark_rep, benchmark_period>(cb);
        end();
    }

    return result / runs;
};
