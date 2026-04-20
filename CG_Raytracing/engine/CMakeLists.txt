add_library(RaytracingEngine STATIC "./include/Shader.hpp" "./src/Shader.cpp")
set_property(TARGET RaytracingEngine PROPERTY CXX_STANDARD 23)

target_include_directories(RaytracingEngine PUBLIC "./include")

if(WIN32)
	target_link_libraries(RaytracingEngine PUBLIC libglew_shared)
elseif(UNIX)
	target_link_libraries(RaytracingEngine PRIVATE 
        GLEW::GLEW 
        OpenGL::GL
    )
endif()