add_executable(UnitTests 
    "./test_main.cpp"
    "./math/test_Vec3.cpp"
)

target_link_libraries(UnitTests PRIVATE Math)

target_include_directories(UnitTests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

if(WIN32)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.14.0
    )
elseif(UNIX)
    find_package(Catch2 3 REQUIRED)
    target_link_libraries(UnitTests PRIVATE Catch2::Catch2WithMain)
endif()

include(CTest)
include(Catch)
catch_discover_tests(UnitTests)
