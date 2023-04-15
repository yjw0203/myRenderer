set(TARGET_NAME rhi)

file(GLOB_RECURSE HEADER_FILES "*.h")
file(GLOB_RECURSE SOURCE_FILES "*.cpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADER_FILES} ${SOURCE_FILES})

add_library(${TARGET_NAME} ${HEADER_FILES} ${SOURCE_FILES})

## 3rd party

target_link_libraries(${TARGET_NAME} PUBLIC ${vulkan_libs})
target_include_directories(${TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${vulkan_include}>)

target_link_libraries(${TARGET_NAME} PUBLIC ${glfw_lib})
target_include_directories(${TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${glfw_include}>)