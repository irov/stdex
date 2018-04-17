cmake_minimum_required(VERSION 3.0)

PROJECT(stdex)

OPTION(STDEX_EXTERNAL_BUILD "STDEX_EXTERNAL_BUILD" OFF)

IF( NOT STDEX_EXTERNAL_BUILD )
    set( CMAKE_CXX_STANDARD 11 )
    
    if(${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
    elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL AppleClang)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
    elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
    elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL Intel)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
    elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Wall /WX")
        set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} /wd4710") #function not inlined 
        set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} /wd4738") #storing 32-bit float result in memory, possible loss of performance
    endif()    

    SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/${CMAKE_GENERATOR} )
    SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_GENERATOR} )    
ENDIF()

MACRO( ADD_FILTER group_name )
	SOURCE_GROUP( ${group_name} FILES ${ARGN} )
	SET( SRC_FILES ${SRC_FILES} ${ARGN} )
ENDMACRO()

SET( INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/stdex )
SET( SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )

ADD_FILTER(
allocator
	${INCLUDE_DIR}/allocator.h
    ${SOURCE_DIR}/allocator.cpp
)

ADD_FILTER(
container
    ${INCLUDE_DIR}/array_string.h
    ${INCLUDE_DIR}/auto_array.h
    ${INCLUDE_DIR}/binary_set.h
    ${INCLUDE_DIR}/binary_vector.h    
    ${INCLUDE_DIR}/heap_array.h
    ${INCLUDE_DIR}/queue.h
    ${INCLUDE_DIR}/unchecked_array_iterator.h
)

ADD_FILTER(
exception
    ${INCLUDE_DIR}/exception.h
    ${SOURCE_DIR}/exception.cpp
)

ADD_FILTER(
intrusive
    ${INCLUDE_DIR}/intrusive_algorithm.h
    ${INCLUDE_DIR}/intrusive_duplex_tree.h
    ${INCLUDE_DIR}/intrusive_linked.h
    ${INCLUDE_DIR}/intrusive_linked_ptr.h
    ${INCLUDE_DIR}/intrusive_list.h
    ${INCLUDE_DIR}/intrusive_ptr.h
    ${INCLUDE_DIR}/intrusive_ptr_base.h
    ${INCLUDE_DIR}/intrusive_slug.h
    ${INCLUDE_DIR}/intrusive_slug_linked.h
    ${INCLUDE_DIR}/intrusive_slug_list.h
    ${INCLUDE_DIR}/intrusive_slug_list_size.h
    ${INCLUDE_DIR}/intrusive_splay_tree.h
    ${INCLUDE_DIR}/intrusive_tree.h
    ${INCLUDE_DIR}/intrusive_tree_base.h
)

ADD_FILTER(
memory
	${INCLUDE_DIR}/memory_cache.h
    ${INCLUDE_DIR}/memory_ptr.h
    ${INCLUDE_DIR}/memory_cache.h
    ${INCLUDE_DIR}/memory_reader.h
    ${INCLUDE_DIR}/memory_writer.h
    ${INCLUDE_DIR}/memorycopy.h
    ${SOURCE_DIR}/memory_cache.cpp
    ${SOURCE_DIR}/memorycopy.cpp
)

ADD_FILTER(
stl
    ${INCLUDE_DIR}/stl_allocator.h
    ${INCLUDE_DIR}/stl_list.h
    ${INCLUDE_DIR}/stl_map.h
    ${INCLUDE_DIR}/stl_string.h
    ${INCLUDE_DIR}/stl_vector.h
)

ADD_FILTER(
sha1
	${INCLUDE_DIR}/sha1.h
    ${SOURCE_DIR}/sha1.cpp
)

ADD_FILTER(
stack
    ${INCLUDE_DIR}/stack.h
	${SOURCE_DIR}/stack.cpp
)

ADD_FILTER(
thread_guard
	${INCLUDE_DIR}/thread_guard.h
    ${SOURCE_DIR}/thread_guard.cpp
)

ADD_FILTER(
xml_sax_parser
    ${INCLUDE_DIR}/xml_sax_parser.h
    ${SOURCE_DIR}/xml_sax_parser.cpp
)

INCLUDE_DIRECTORIES( ${PROJECT_NAME} include ) 

ADD_LIBRARY( ${PROJECT_NAME} STATIC ${SRC_FILES} )