

file(GLOB_RECURSE HEADERS "*.h")
file(GLOB_RECURSE SOURCES "*.cpp")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(PROJECT_PATH ${PROJECT_SOURCE_DIR})
set(SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(SHADER_PATH ${PROJECT_SOURCE_DIR}/shaders)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/project/projectInfo.h.in ${CMAKE_CURRENT_SOURCE_DIR}/generate/projectInfo.h)

add_subdirectory(rhi)

add_subdirectory(client)
