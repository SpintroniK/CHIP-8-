#pragma once

#include <cstdint>
#include <limits>
#include <numbers>
#include <ranges>
#include <vector>

namespace Sound
{
    
    template <typename T>
    auto GenerateSound(double freq, std::size_t sampleRate) -> std::vector<T>
    {

        using namespace std::numbers;

        // std::vector<T> soundData(sampleRate);

        const auto radiansPerSample = freq * 2 * pi / static_cast<double>(sampleRate);

        const auto amplitude = static_cast<double>(std::numeric_limits<T>::max());

        const auto v = std::views::iota(0) 
                    | std::views::take(sampleRate)
                    | std::views::transform([&] (const auto& x) { return radiansPerSample * x; })
                    | std::views::transform([] (const auto& x) { return std::sin(x); })
                    | std::views::transform([&] (const auto& x) { return amplitude * x; })
                    | std::views::common;

        return std::vector<std::int16_t>(v.begin(), v.end());
    }

} // namespace Sound
