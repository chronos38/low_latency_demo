#pragma once

#include <string>
#include <chrono>
#include <sstream>
#include <cstddef>
#include <algorithm>
#include <map>
#include <vector>
#include <unordered_map>
#include <cstdint>

class exporter
{
public:
    static constexpr char separator = ';';

    template <class Rep, class Period>
    static std::string execute(int32_t size, const std::vector<std::string>& ids,
                               const std::vector<std::chrono::duration<Rep, Period>>& results)
    {
        std::ostringstream stream;
        for (size_t i = 0; i < ids.size(); ++i) {
            stream << size << separator << ids[i] << separator << line(results[i]) << std::endl;
        }
        stream << std::endl;
        return stream.str();
    }

private:
    template <class Rep, class Period>
    static std::string line(const std::chrono::duration<Rep, Period>& duration)
    {
        const std::unordered_map<intmax_t, std::string> mapping = {
            std::make_pair(1, "s"), std::make_pair(std::milli::den, "ms"), std::make_pair(std::micro::den, "us"),
            std::make_pair(std::nano::den, "ns")};
        std::ostringstream stream;
        stream << duration.count() << ' ' << mapping.at(Period::den);
        return stream.str();
    }
};
