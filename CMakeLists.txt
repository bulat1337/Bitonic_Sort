cmake_minimum_required(VERSION 3.14)

cmake_policy(SET CMP0091 NEW)

project(BitonicSort LANGUAGES CXX)

find_package(OpenCL REQUIRED)

project(BitonicSort)

enable_testing()

option(ENABLE_LOGGING "Enable logging" OFF)
set(ENABLE_LOGGING ${ENABLE_LOGGING} CACHE BOOL "Enable logging" FORCE)

option(PRINT_DURATION "Enable logging" OFF)
set(PRINT_DURATION ${PRINT_DURATION} CACHE BOOL "Print duration" FORCE)

option(SORT_ON_CPU "Sort on CPU" OFF)
set(SORT_ON_CPU ${SORT_ON_CPU} CACHE BOOL "Sort on CPU" FORCE)

option(ENABLE_BD_TESTS "Enables big data tests" OFF)
set(ENABLE_BD_TESTS ${ENABLE_BD_TESTS} CACHE BOOL "Enables big data tests" FORCE)

set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Build type")
add_subdirectory(unit_tests/)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(COPY ${CMAKE_SOURCE_DIR}/kernels/bitonic_sort.cl DESTINATION ${CMAKE_BINARY_DIR})

if(MSVC)
    set(RELEASE_COMPILE_OPTIONS
        /O2
        /W4
        /DNDEBUG
    )

    set(DEBUG_COMPILE_OPTIONS
        /Od
        /RTC1
        /W4
        /D_DEBUG
    )
else()
    set(RELEASE_COMPILE_OPTIONS
        -O2
        -fstack-protector
        -fPIE
        -fstrict-overflow
        -Wformat-security
        -Wformat=2
        -Wall
        -Wextra
        -Wnon-virtual-dtor
        -Woverloaded-virtual
        -Wcast-align
        -Wpointer-arith
        -Wunreachable-code
        -Wno-pre-c++17-compat
    )

    set(DEBUG_COMPILE_OPTIONS
        -D_DEBUG
        -ggdb3
        -O0
        -Wall
        -Wextra
        -Wc++14-compat
        -Wmissing-declarations
        -Wcast-align
        -Wcast-qual
        -Wchar-subscripts
        -Wconversion
        -Wctor-dtor-privacy
        -Wempty-body
        -Wfloat-equal
        -Wformat-security
        -Winline
        -Wnon-virtual-dtor
        -Woverloaded-virtual
        -Wpacked
        -Wpointer-arith
        -Winit-self
        -Wredundant-decls
        -Wshadow
        -Wsign-conversion
        -Wsign-promo
        -Wstrict-overflow=2
        -Wsuggest-override
        -Wswitch-default
        -Wswitch-enum
        -Wundef
        -Wunreachable-code
        -Wunused
        -Wvariadic-macros
        -Wno-missing-field-initializers
        -Wno-narrowing
        -Wno-old-style-cast
        -Wno-varargs
        -Wstack-protector
        -fcheck-new
        -fsized-deallocation
        -fstack-protector
        -fstrict-overflow
        -fno-omit-frame-pointer
        -Wlarger-than=8192
        -fPIE
        -Werror=vla
        -Wno-pre-c++17-compat
        -Wno-pre-c++20-compat
        -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
    )
endif()


# ----- executable -----

add_executable(bitonic_sort.x ./main.cpp)
target_include_directories(bitonic_sort.x PRIVATE
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/include/detail
	${CMAKE_SOURCE_DIR}/utils/include
	${CMAKE_SOURCE_DIR}/RB_Tree/include
	${OpenCL_INCLUDE_DIRS})

target_link_libraries(bitonic_sort.x PRIVATE ${OpenCL_LIBRARIES})

if(ENABLE_LOGGING)
	target_compile_definitions(bitonic_sort.x PRIVATE ENABLE_LOGGING)
endif()

if(SORT_ON_CPU)
	target_compile_definitions(bitonic_sort.x PRIVATE SORT_ON_CPU)
endif()

if(PRINT_DURATION)
	target_compile_definitions(bitonic_sort.x PRIVATE PRINT_DURATION)
endif()

target_compile_options(bitonic_sort.x PRIVATE
    $<$<CONFIG:Debug>:${DEBUG_COMPILE_OPTIONS}>
    $<$<CONFIG:Release>:${RELEASE_COMPILE_OPTIONS}>
)

target_compile_definitions(bitonic_sort.x PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
)

target_link_options(bitonic_sort.x PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr>
)
