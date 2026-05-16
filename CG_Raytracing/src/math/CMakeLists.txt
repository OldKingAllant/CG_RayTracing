add_library(Math STATIC
    "./src/ray.cpp"
    "./src/vec3.cpp"
    "./include/ray.hpp"
    "./include/vec3.hpp"
    "./include/random_prob.hpp"
 "src/random_prob.cpp")

set_property(TARGET Math PROPERTY CXX_STANDARD 23)

target_include_directories(Math PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

# target_link_libraries(Math PUBLIC common)
