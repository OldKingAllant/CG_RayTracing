add_library(Geometry STATIC
    "src/geometry.cpp"
    "include/geometry.hpp"
)
target_include_directories(Geometry PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

set_property(TARGET Geometry PROPERTY CXX_STANDARD 23)

target_include_directories(Geometry PUBLIC "./include/")

target_link_libraries(Geometry PUBLIC common)
