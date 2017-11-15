#pragma once

#include <chrono>

#define KB (1024)
#define MB (1024 * KB)
#define GB (1024 * MB)

typedef std::chrono::duration<double, std::milli> benchmark_result;
typedef benchmark_result::rep benchmark_rep;
typedef benchmark_result::period benchmark_period;
