add_library(Scene STATIC
    "src/camera.cpp"
    "include/camera.hpp"
)

set_property(TARGET Scene PROPERTY CXX_STANDARD 23)

target_include_directories(Scene PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(Scene PUBLIC common Math RaytracingEngine)
