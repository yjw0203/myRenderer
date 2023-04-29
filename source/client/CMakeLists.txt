set(TARGET_NAME Renderer)

file(GLOB_RECURSE HEADER_FILES "*.h")
file(GLOB_RECURSE SOURCE_FILES "*.cpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADER_FILES} ${SOURCE_FILES})

## executable
add_executable(${TARGET_NAME} ${HEADERS} ${SOURCES})

target_link_libraries(${TARGET_NAME} PUBLIC ${glfw_lib})
target_include_directories(${TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${glm_include}>)
target_include_directories(${TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${glfw_include}>)


target_link_libraries(${TARGET_NAME} PUBLIC rhi)

add_dependencies(Renderer rhi)