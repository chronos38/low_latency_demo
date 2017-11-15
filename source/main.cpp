#include <demo/color.h>
#include <demo/memcpy.h>

#include <boost/simd.hpp>
#include <boost/simd/function/load.hpp>
#include <boost/simd/function/store.hpp>

#include <intrin.h>
#include <omp.h>

#include <cstdint>
#include <cstring>
#include <memory>
#include <fstream>

bench::time_point bench::start_;

int main(int, char**)
{
    std::ofstream stream;
    omp_set_num_threads(8);

    stream.open("memory.csv");
    run_memory_benchmark(stream);
    stream.close();

    stream.open("color.csv");
    run_color_benchmark(stream);
    stream.close();

    return 0;
}

void* memcpy_parallel(void* destination, const void* source, size_t num)
{
    auto dst = reinterpret_cast<int8_t*>(destination);
    auto src = reinterpret_cast<const int8_t*>(source);

#pragma omp parallel for
    for (int i = 0; i < num; ++i) {
        dst[i] = src[i];
    }

    return destination;
}

void* memcpy_vector(void* destination, const void* source, size_t num)
{
    const char* src = (const char*)source;
    char* dst = (char*)destination;
    size_t i;

    if (!((uintptr_t)src & 15) && !((uintptr_t)dst & 15)) {
        __m128 values[4];
        for (i = num / 64; i--;) {
            _mm_prefetch(src, _MM_HINT_NTA);
            values[0] = *(__m128*)(src + 0);
            values[1] = *(__m128*)(src + 16);
            values[2] = *(__m128*)(src + 32);
            values[3] = *(__m128*)(src + 48);
            _mm_stream_ps((float*)(dst + 0), values[0]);
            _mm_stream_ps((float*)(dst + 16), values[1]);
            _mm_stream_ps((float*)(dst + 32), values[2]);
            _mm_stream_ps((float*)(dst + 48), values[3]);
            src += 64;
            dst += 64;
        }
        num &= 63;
    }

    while (num--) {
        *dst++ = *src++;
    }
    return destination;
}

template <class T>
constexpr T clip(T value, T min, T max)
{
    return std::min(std::max(value, min), max);
}

std::unique_ptr<float[]> yuv_to_rgba(const float* yuv, int size)
{
    const int rgba_size = size / 3 * 4;
    auto rgba = std::make_unique<float[]>(rgba_size);

    for (int i = 0, j = 0; i < size && j < rgba_size; i += 3, j += 4) {
        const auto y = yuv[i];
        const auto u = yuv[i + 1];
        const auto v = yuv[i + 2];
        rgba[j] = clip(y + 1.140f * v, .0f, 1.f);
        rgba[j + 1] = clip(y - 0.395f * u - 0.581f * v, .0f, 1.f);
        rgba[j + 2] = clip(y + 2.032f * u, .0f, 1.f);
        rgba[j + 3] = 1.f;
    }

    return rgba;
}

std::unique_ptr<float[]> yuv_to_rgba_parallel(const float* yuv, int size)
{
    const int pixel_size = size / 3;
    const int rgba_size = size / 3 * 4;
    auto rgba = std::make_unique<float[]>(rgba_size);

#pragma omp parallel for
    for (int p = 0; p < pixel_size; ++p) {
        const auto i = p * 3;
        const auto j = p * 4;
        const auto y = yuv[i];
        const auto u = yuv[i + 1];
        const auto v = yuv[i + 2];
        rgba[j] = clip(y + 1.140f * v, .0f, 1.f);
        rgba[j + 1] = clip(y - 0.395f * u - 0.581f * v, .0f, 1.f);
        rgba[j + 2] = clip(y + 2.032f * u, .0f, 1.f);
        rgba[j + 3] = 1.f;
    }

    return rgba;
}

std::unique_ptr<float[]> yuv_to_rgba_vector(const float* yuv, int size)
{
    static_assert(boost::simd::pack<float>::static_size == 4, "boost pack is not of size 4");
    const int pixel_size = size / 3;
    const int rgba_size = size / 3 * 4;
    auto rgba = std::make_unique<float[]>(rgba_size);
    std::array<float, 4> r_store;
    std::array<float, 4> g_store;
    std::array<float, 4> b_store;

    for (int i = 0, j = 0; i < size && j < rgba_size; i += 3 * 4, j += 4 * 4) {
        boost::simd::pack<float, 4> y{yuv[i], yuv[i + 3], yuv[i + 6], yuv[i + 9]};
        boost::simd::pack<float, 4> u{yuv[i + 1], yuv[i + 4], yuv[i + 7], yuv[i + 10]};
        boost::simd::pack<float, 4> v{yuv[i + 2], yuv[i + 5], yuv[i + 8], yuv[i + 11]};
        auto r = y + 1.140f * v;
        auto g = y - 0.395f * u - 0.581f * v;
        auto b = y + 2.032f * u;
        boost::simd::store(r, r_store.data());
        boost::simd::store(g, g_store.data());
        boost::simd::store(b, b_store.data());

        for (int n = 0; n < 4; ++n) {
            rgba[j + n * 4] = clip(r_store[n], .0f, 1.f);
            rgba[j + n * 4 + 1] = clip(g_store[n], .0f, 1.f);
            rgba[j + n * 4 + 2] = clip(b_store[n], .0f, 1.f);
            rgba[j + n * 4 + 3] = 1.f;
        }
    }

    return rgba;
}

std::unique_ptr<float[]> yuv_to_rgba_parallel_vector(const float* yuv, int size)
{
    static_assert(boost::simd::pack<float>::static_size == 4, "boost pack is not of size 4");
    const int pixel_size = size / 3;
    const int rgba_size = size / 3 * 4;
    auto rgba = std::make_unique<float[]>(rgba_size);
    std::array<float, 4> r_store;
    std::array<float, 4> g_store;
    std::array<float, 4> b_store;

#pragma omp parallel for
    for (int p = 0; p < pixel_size; ++p) {
        const auto i = p * 3;
        const auto j = p * 4;
        boost::simd::pack<float, 4> y{yuv[i], yuv[i + 3], yuv[i + 6], yuv[i + 9]};
        boost::simd::pack<float, 4> u{yuv[i + 1], yuv[i + 4], yuv[i + 7], yuv[i + 10]};
        boost::simd::pack<float, 4> v{yuv[i + 2], yuv[i + 5], yuv[i + 8], yuv[i + 11]};
        auto r = y + 1.140f * v;
        auto g = y - 0.395f * u - 0.581f * v;
        auto b = y + 2.032f * u;
        boost::simd::store(r, r_store.data());
        boost::simd::store(g, g_store.data());
        boost::simd::store(b, b_store.data());

        for (int n = 0; n < 4; ++n) {
            rgba[j + n * 4] = clip(r_store[n], .0f, 1.f);
            rgba[j + n * 4 + 1] = clip(g_store[n], .0f, 1.f);
            rgba[j + n * 4 + 2] = clip(b_store[n], .0f, 1.f);
            rgba[j + n * 4 + 3] = 1.f;
        }
    }

    return rgba;
}
