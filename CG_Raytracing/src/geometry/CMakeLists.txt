add_library(Geometry STATIC
    "src/geometry.cpp"
    "src/cube.cpp"
    "src/triangle.cpp"
    "src/sphere.cpp"
    "src/hit_record.cpp"
)

target_include_directories(Geometry PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

set_property(TARGET Geometry PROPERTY CXX_STANDARD 23)

target_include_directories(Geometry PUBLIC "./include/")

target_link_libraries(Geometry PUBLIC common Math)
