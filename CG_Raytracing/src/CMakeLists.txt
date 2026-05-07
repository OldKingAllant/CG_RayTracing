add_library(CoreLogic STATIC
    "scene/include/camera.hpp"
    "scene/src/camera.cpp"
)

set_property(TARGET CoreLogic PROPERTY CXX_STANDARD 23)

target_include_directories(CoreLogic PUBLIC "./scene/include/")

target_link_libraries(CoreLogic PUBLIC common)
