cmake_minimum_required(VERSION 3.5)

option(STDEX_INSTALL "STDEX_INSTALL" ON)

project(stdex)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

if(STDEX_INSTALL)
    install(DIRECTORY include/stdex
        DESTINATION include
        FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h")

    install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets)
endif()
