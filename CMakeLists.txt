cmake_minimum_required( VERSION 3.10 )

message( STATUS "==========================================================================\n" )
message( STATUS "project 'Renderer' generation started" )

message( STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} (${CMAKE_CXX_COMPILER_VERSION})" )
message( STATUS "target system: ${CMAKE_SYSTEM_NAME} (${CMAKE_SYSTEM_VERSION})" )
message( STATUS "host system: ${CMAKE_HOST_SYSTEM_NAME} (${CMAKE_HOST_SYSTEM_VERSION})" )

project( "Renderer" VERSION 2023.1.31 LANGUAGES C CXX )

## language standard
set(CMAKE_CXX_STANDARD 17)

## 3rd party
set(vulkan_lib_dir ${CMAKE_CURRENT_SOURCE_DIR}/3rd/VulkanSDK/1.2.170.0/Lib)
file(GLOB VULKAN_LIB_FILES ${vulkan_lib_dir}/*d.lib)
set(vulkan_include ${CMAKE_CURRENT_SOURCE_DIR}/3rd/VulkanSDK/1.2.170.0/Include)
set(vulkan_libs ${VULKAN_LIB_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/3rd/VulkanSDK/1.2.170.0/Lib/vulkan-1.lib)

set(glfw_include ${CMAKE_CURRENT_SOURCE_DIR}/3rd/glfw/include)
set(glfw_lib ${CMAKE_CURRENT_SOURCE_DIR}/3rd/glfw/lib-vc2022/glfw3.lib)

set(stb_include ${CMAKE_CURRENT_SOURCE_DIR}/3rd/stb/include)

set(glm_include ${CMAKE_CURRENT_SOURCE_DIR}/3rd/glm-0.9.9.8)

add_subdirectory(source)
