set(module_name_list "")
set(module_public_headers_path "")
set(module_dependencys "")

set(INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/generate1" ${3rd_party_include} ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE ModuleJsonFileList "*/module.json")
foreach(ModuleJsonFile IN LISTS ModuleJsonFileList)
    file(READ "${ModuleJsonFile}" json_string)
    string(REGEX REPLACE "/module.json" "" ModulePath ${ModuleJsonFile})
    string(JSON ModuleName ERROR_VARIABLE "NOT_FOUND_NAME" GET ${json_string} name)
    string(JSON ModuleBuild ERROR_VARIABLE "lib" GET ${json_string} build)
    string(JSON DependencysStr ERROR_VARIABLE "" GET ${json_string} dependencys)
    file(GLOB_RECURSE all_headers "${ModulePath}/*.h")
    file(GLOB_RECURSE all_hpps "${ModulePath}/*.hpp")
    file(GLOB_RECURSE all_sources "${ModulePath}/*.cpp")
    file(GLOB_RECURSE public_headers "${ModulePath}/Public/*.h")

    get_filename_component(ModuleLastPath ${ModulePath} DIRECTORY)

    source_group(TREE "${ModulePath}" FILES ${all_headers} ${all_sources} ${all_hpps} ${ModuleJsonFile})

    if(ModuleBuild STREQUAL "lib")
        add_library(${ModuleName} ${all_headers} ${all_sources} ${all_hpps})
    elseif(ModuleBuild STREQUAL "dll")
        add_library(${ModuleName} SHARED ${all_headers} ${all_sources} ${all_hpps})
    elseif(ModuleBuild STREQUAL "exe")
        add_executable(${ModuleName} ${all_headers} ${all_sources} ${all_hpps})
        target_link_libraries(${ModuleName} PRIVATE ${3rd_party_lib})

        add_custom_command(TARGET ${ModuleName} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${3rd_party_dll}
                $<TARGET_FILE_DIR:${ModuleName}>)
    elseif(ModuleBuild STREQUAL "py_module")
        set(pybind11_DIR ${pybind_tools})
        find_package(pybind11 REQUIRED)
        pybind11_add_module(${ModuleName} ${all_headers} ${all_sources} ${all_hpps})
        target_link_libraries(${ModuleName} PRIVATE ${3rd_party_lib})
        message(STATUS "Add_Pybind_Module: py${ModuleName}")
    else()
        message(FATAL_ERROR "Unknown BuildOption: ${ModuleBuild}")
    endif()

    file(RELATIVE_PATH FILE_RELATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${ModuleLastPath})
    set_target_properties(${ModuleName} PROPERTIES FOLDER "Modules/${FILE_RELATIVE_PATH}")

    target_sources(${ModuleName} PRIVATE ${ModuleJsonFile})

    target_include_directories(${ModuleName} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/generate1")
    target_include_directories(${ModuleName} PRIVATE ${3rd_party_include})
    target_include_directories(${ModuleName} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

    list(APPEND module_name_list ${ModuleName})
    list(APPEND module_public_headers_path ${ModulePath}/Public)
    list(APPEND module_dependencys ${DependencysStr})

    message(STATUS "Module_Json: ${ModuleJsonFile}")
    message(STATUS "ModuleName: ${ModuleName}")
    message(STATUS "ModulePath: ${ModulePath}")
    message(STATUS "ModuleLastPath: ${ModuleLastPath}")
    message(STATUS "ModuleDependencys: ${Dependencys}")
endforeach(ModuleJsonFile)

message(STATUS "module_dependency: ${module_dependencys}")
message(STATUS "-------------------------------------------------------------")

foreach(ModuleName IN LISTS module_name_list)
    message(STATUS "Begin resolve dependency of Module: ${ModuleName}")
    list(FIND module_name_list ${ModuleName} ModuleIndex)
    list(GET module_dependencys ${ModuleIndex} DependencysStr)

    message(${DependencysStr})
    string(REGEX MATCHALL "\"([^\"]+)\"" DependencysStr ${DependencysStr})
    string(LENGTH "${DependencysStr}" length)
    if(length EQUAL 0)
        continue()
    endif()

    string(REPLACE "\"" " " Dependencys ${DependencysStr})
    separate_arguments(Dependencys)

    message(STATUS "----ModuleDependency: ${Dependencys}")
    foreach(ModuleDependencyName IN LISTS Dependencys)
        string(LENGTH "${ModuleDependencyName}" length)
        if(length EQUAL 0)
            continue()
        endif()
        message(STATUS "--------DependencyModuleName: ${ModuleDependencyName}")
        list(FIND module_name_list ${ModuleDependencyName} ModuleDependencyIndex)
        list(GET module_public_headers_path ${ModuleDependencyIndex} dependencyPublicHeaders)
        target_link_libraries(${ModuleName} PRIVATE ${ModuleDependencyName})
    endforeach(ModuleDependencyName)
endforeach(ModuleName)

set(PROJECT_PATH ${PROJECT_SOURCE_DIR})
set(SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(SHADER_PATH ${PROJECT_SOURCE_DIR}/shaders)
set(RESOURCE_PATH ${PROJECT_SOURCE_DIR}/resource)
set(CR_TEMPLATE_PATH ${SOURCE_PATH}/Tools/CppReflection/Private/Template)
set(CR_GENERATE_PUBLIC_PATH ${SOURCE_PATH}/Generate/Public)
set(CR_GENERATE_PRIVATE_PATH ${SOURCE_PATH}/Generate/Private)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/project/projectInfo.h.in ${CMAKE_CURRENT_SOURCE_DIR}/generate1/projectInfo.h)