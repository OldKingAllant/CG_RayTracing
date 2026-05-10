add_library(common STATIC "./include/Utility.hpp" "./include/config.hpp" "./src/Utility.cpp")
target_include_directories(common PUBLIC "./include")
