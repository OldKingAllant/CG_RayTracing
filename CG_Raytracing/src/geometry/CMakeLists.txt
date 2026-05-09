add_library(Geometry STATIC
    "src/geometry.cpp"
    "include/geometry.hpp"
)

set_property(TARGET Geometry PROPERTY CXX_STANDARD 23)

target_include_directories(Geometry PUBLIC "./include/")

target_link_libraries(Geometry PUBLIC common)
