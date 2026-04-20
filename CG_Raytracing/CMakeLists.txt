add_subdirectory("./app")
add_subdirectory("./engine")
add_subdirectory("./common")

target_link_libraries(CG_Raytracing PUBLIC RaytracingEngine)
target_link_libraries(CG_Raytracing PUBLIC common)

target_link_libraries(RaytracingEngine PUBLIC common)