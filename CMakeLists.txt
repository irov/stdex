cmake_minimum_required(VERSION 3.0)

OPTION(STDEX_EXTERNAL_BUILD "STDEX_EXTERNAL_BUILD" OFF)
OPTION(STDEX_INSTALL "STDEX_INSTALL" ON)

PROJECT(stdex)

IF(NOT STDEX_EXTERNAL_BUILD)
    set( CMAKE_CXX_STANDARD 17 )
    
    if(${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Werror")
    elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL AppleClang)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Werror")
    elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Werror")
    elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL Intel)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Werror")
    elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4710") #function not inlined 
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4738") #storing 32-bit float result in memory, possible loss of performance
    endif()   
    
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS)
ENDIF()

SET(STDEX_DBGHELP_INCLUDE "" CACHE STRING "STDEX_DBGHELP_INCLUDE")

MACRO(ADD_FILTER group_name)
    SOURCE_GROUP( ${group_name} FILES ${ARGN} )
    SET( SRC_FILES ${SRC_FILES} ${ARGN} )
ENDMACRO()

SET(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/stdex)
SET(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

ADD_FILTER(
utils
    ${INCLUDE_DIR}/typename.h
    ${INCLUDE_DIR}/span.h
)

ADD_FILTER(
container
    ${INCLUDE_DIR}/array_string.h
    ${INCLUDE_DIR}/auto_array.h
    ${INCLUDE_DIR}/binary_set.h
    ${INCLUDE_DIR}/binary_vector.h
    ${INCLUDE_DIR}/static_array.h
    ${INCLUDE_DIR}/dynamic_array.h
    ${INCLUDE_DIR}/static_hash_array.h
    ${INCLUDE_DIR}/queue.h
    ${INCLUDE_DIR}/unchecked_array_iterator.h
)

ADD_FILTER(
pool
    ${INCLUDE_DIR}/pool.h
    ${INCLUDE_DIR}/pool2.h
)

ADD_FILTER(
intrusive
    ${INCLUDE_DIR}/intrusive_algorithm.h
    ${INCLUDE_DIR}/intrusive_duplex_tree.h
    ${INCLUDE_DIR}/intrusive_linked.h
    ${INCLUDE_DIR}/intrusive_linked_ptr.h
    ${INCLUDE_DIR}/intrusive_list.h
    ${INCLUDE_DIR}/intrusive_list_ptr.h
    ${INCLUDE_DIR}/intrusive_ptr_config.h
    ${INCLUDE_DIR}/intrusive_ptr.h
    ${INCLUDE_DIR}/intrusive_ptr_base.h
    ${INCLUDE_DIR}/intrusive_ptr_scope.h
    ${INCLUDE_DIR}/intrusive_ptr_view.h
    ${INCLUDE_DIR}/intrusive_slug.h
    ${INCLUDE_DIR}/intrusive_slug_ptr.h
    ${INCLUDE_DIR}/intrusive_slug_linked.h
    ${INCLUDE_DIR}/intrusive_slug_linked_ptr.h
    ${INCLUDE_DIR}/intrusive_slug_linked_tag.h
    ${INCLUDE_DIR}/intrusive_slug_list.h
    ${INCLUDE_DIR}/intrusive_slug_list_ptr.h
    ${INCLUDE_DIR}/intrusive_slug_list_size.h    
    ${INCLUDE_DIR}/intrusive_slug_list_size_ptr.h
    ${INCLUDE_DIR}/intrusive_splay_tree.h
    ${INCLUDE_DIR}/intrusive_tree.h
    ${INCLUDE_DIR}/intrusive_tree_base.h
)

ADD_FILTER(
memory
    ${INCLUDE_DIR}/memory_ptr.h
    ${INCLUDE_DIR}/memory_reader.h
    ${INCLUDE_DIR}/memory_writer.h
    ${INCLUDE_DIR}/container_writer.h
    ${INCLUDE_DIR}/container_reader.h
    ${INCLUDE_DIR}/memorycopy.h
    ${SOURCE_DIR}/memorycopy.cpp
)

ADD_FILTER(
sha1
    ${INCLUDE_DIR}/sha1.h
    ${SOURCE_DIR}/sha1.cpp
)

INCLUDE_DIRECTORIES(${PROJECT_NAME} ${STDEX_DBGHELP_INCLUDE} ${CMAKE_CURRENT_SOURCE_DIR}/include)

ADD_LIBRARY(${PROJECT_NAME} STATIC ${SRC_FILES})

if(STDEX_INSTALL)
    install(DIRECTORY include/stdex 
        DESTINATION include
        FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h")

    install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
endif()