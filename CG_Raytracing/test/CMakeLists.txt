add_executable(UnitTests 
    main.cpp
)

target_link_libraries(UnitTests PRIVATE 
    engine 
    scene 
    common
)

target_include_directories(UnitTests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

# enable_testing()
# add_test(NAME AllProjectTests COMMAND UnitTests)
