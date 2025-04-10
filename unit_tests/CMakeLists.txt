cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(TEST_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/")
add_definitions(-DTEST_DATA_DIR=\"${TEST_DATA_DIR}\")

project(Testing LANGUAGES CXX)

find_package(OpenCL REQUIRED)
find_package(GTest REQUIRED)

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

add_executable(unit_tests
	src/unit_tests.cpp
	main.cpp
)

target_compile_options(unit_tests PRIVATE
    $<$<CONFIG:Debug>:${DEBUG_COMPILE_OPTIONS}>
    $<$<CONFIG:Release>:${RELEASE_COMPILE_OPTIONS}>
)

target_compile_definitions(unit_tests PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
)

target_link_options(unit_tests PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr>
)

target_link_libraries(unit_tests
	GTest::GTest
	GTest::Main
	${OpenCL_LIBRARIES}
)

target_include_directories(unit_tests PRIVATE
	${CMAKE_SOURCE_DIR}/unit_tests/test_utils/include
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/include/detail
	${CMAKE_SOURCE_DIR}/utils/include
	${OpenCL_INCLUDE_DIRS}
)

if(ENABLE_BD_TESTS)
    target_compile_definitions(unit_tests PRIVATE ENABLE_BD_TESTS)
endif()

enable_testing()
add_test(NAME UnitTests COMMAND unit_tests)
