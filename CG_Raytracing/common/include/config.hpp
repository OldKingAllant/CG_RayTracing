#pragma once

namespace Config {
inline constexpr double ASPECT_RATIO = 16.0 / 9.0;
inline constexpr int IMAGE_WIDTH = 400;
inline constexpr int IMAGE_HEIGHT =
    static_cast<int>(IMAGE_WIDTH / ASPECT_RATIO);

inline constexpr double FOCAL_LENGTH = 50;

inline constexpr int RAY_PER_PIXEL = 10;
inline constexpr int RENDER_ITERATION = 10;
} // namespace Config
