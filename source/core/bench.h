#pragma once

#include <chrono>
#include <functional>

class bench
{
public:
    typedef std::chrono::high_resolution_clock clock;
    typedef clock::duration duration;
    typedef clock::period period;
    typedef clock::time_point time_point;
    typedef clock::rep rep;
    typedef std::function<void()> callback;

    template <class Rep, class Period>
    static std::chrono::duration<Rep, Period> run(const callback& cb)
    {
        typedef std::chrono::duration<Rep, Period> result;
        begin();
        cb();
        return std::chrono::duration_cast<result>(end());
    }

private:
    static time_point start_;

    static void begin()
    {
        start_ = clock::now();
    }

    static duration end()
    {
        return clock::now() - start_;
    }
};
