cmake_minimum_required( VERSION 3.18 )

message( STATUS "==========================================================================\n" )
message( STATUS "project 'Renderer' generation started" )

message( STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} (${CMAKE_CXX_COMPILER_VERSION})" )
message( STATUS "target system: ${CMAKE_SYSTEM_NAME} (${CMAKE_SYSTEM_VERSION})" )
message( STATUS "host system: ${CMAKE_HOST_SYSTEM_NAME} (${CMAKE_HOST_SYSTEM_VERSION})" )

project( "Renderer" VERSION 2023.1.31 LANGUAGES C CXX )

## language standard
set(CMAKE_CXX_STANDARD 17)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

## 3rd party
set(vulkan_lib_dir ${CMAKE_CURRENT_SOURCE_DIR}/3rd/VulkanSDK/1.2.198.1/Lib)
file(GLOB VULKAN_LIB_FILES ${vulkan_lib_dir}/*d.lib)
set(vulkan_include ${CMAKE_CURRENT_SOURCE_DIR}/3rd/VulkanSDK/1.2.198.1/Include)

set(vulkan_libs ${VULKAN_LIB_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/3rd/VulkanSDK/1.2.198.1/Lib/vulkan-1.lib)

set(glfw_include ${CMAKE_CURRENT_SOURCE_DIR}/3rd/glfw/include)
set(glfw_lib ${CMAKE_CURRENT_SOURCE_DIR}/3rd/glfw/lib-vc2022/glfw3.lib)

set(stb_include ${CMAKE_CURRENT_SOURCE_DIR}/3rd/stb/include)

set(glm_include ${CMAKE_CURRENT_SOURCE_DIR}/3rd/glm-0.9.9.8)

set(shaderc_include ${CMAKE_CURRENT_SOURCE_DIR}/3rd/shaderc/include)
set(shaderc_lib ${CMAKE_CURRENT_SOURCE_DIR}/3rd/shaderc/lib/ShaderConductor.lib)
set(shaderc_dll ${CMAKE_CURRENT_SOURCE_DIR}/3rd/shaderc/bin/ShaderConductor.dll)

set(rapid_json_include ${CMAKE_CURRENT_SOURCE_DIR}/3rd/rapidjson/include)

set(json_include ${CMAKE_CURRENT_SOURCE_DIR}/3rd/json/include)

set(llvm_include ${CMAKE_CURRENT_SOURCE_DIR}/3rd/LLVM/include)
set(llvm_lib ${CMAKE_CURRENT_SOURCE_DIR}/3rd/LLVM/lib/x64/libclang.lib)
set(llvm_dll ${CMAKE_CURRENT_SOURCE_DIR}/3rd/LLVM/bin/x64/libclang.dll)

set(mustache_include ${CMAKE_CURRENT_SOURCE_DIR}/3rd/mustache/include)

set(pybind_include ${CMAKE_CURRENT_SOURCE_DIR}/3rd/pybind/include)

set(3rd_party_include ${vulkan_include} ${glfw_include} ${stb_include} ${glm_include} ${shaderc_include} ${rapid_json_include} ${json_include} ${llvm_include} ${mustache_include} ${pybind_include})
set(3rd_party_lib ${vulkan_libs} ${glfw_lib} ${shaderc_lib} ${llvm_lib})
set(3rd_party_dll ${shaderc_dll} ${llvm_dll})

add_subdirectory(source)
add_subdirectory(shaders)