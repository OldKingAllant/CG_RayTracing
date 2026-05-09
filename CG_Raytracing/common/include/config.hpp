#pragma once

#include <cstdint>
namespace Config {
inline constexpr float ASPECT_RATIO = 4.0 / 3.0;
inline constexpr uint32_t IMAGE_WIDTH = 400;
inline constexpr uint32_t IMAGE_HEIGHT =
    static_cast<uint32_t>(IMAGE_WIDTH / ASPECT_RATIO);

inline constexpr uint32_t FOCAL_LENGTH = 50; // focal length in mm
inline constexpr float SENSOR_SIZE_WIDTH =
    36.0; // sensor size in mm (36 is full-frame)

inline constexpr float CAMERA_POSITION[3] = {0.0, 0.0, 0.0};
inline constexpr float CAMERA_DIRECTION[3] = {1.0, 0.0, 0.0};

inline constexpr uint32_t RAY_PER_PIXEL = 10;
inline constexpr uint32_t RENDER_ITERATION = 10;
} // namespace Config
