set(TARGET_NAME Test)

file(GLOB_RECURSE TEST_HEADER_FILES "*.h")
file(GLOB_RECURSE TEST_SOURCE_FILES "*.cpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${TEST_HEADER_FILES} ${TEST_SOURCE_FILES})

## executable
add_executable(${TARGET_NAME} ${TEST_HEADER_FILES} ${TEST_SOURCE_FILES})

target_link_libraries(${TARGET_NAME} PUBLIC ${glfw_lib})
target_include_directories(${TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${glm_include}>)
target_include_directories(${TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${glfw_include}>)


target_link_libraries(${TARGET_NAME} PUBLIC RHI)

add_dependencies(Renderer RHI)