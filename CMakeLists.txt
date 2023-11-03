# File: CMakeLists.txt
#
# This file is a part of cpplib project which is distributed under MIT License.
# See file LICENSE for full license details.
#
# Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>

cmake_minimum_required(VERSION 3.21.3)

project(cpplib
	LANGUAGES CXX
	VERSION 0.1
	DESCRIPTION "C++ boilerplate library"
	)

######################################### PLATFORM #######################################

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(CPPLIB_COMPILER_CLANG_OR_GCC TRUE)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set(CPPLIB_COMPILER_MSVC TRUE)
else()
	message(WARNING "Unknown compiler: ${CMAKE_CXX_COMPILER_ID}")
endif()

if(WIN32 OR CYGWIN OR MINGW)
	set(CPPLIB_PLATFORM_WINDOWS TRUE)
elseif(UNIX)
	set(CPPLIB_PLATFORM_UNIX TRUE)
else()
	message(WARNING "Unknown platform")
endif()

######################################### HELPERS ########################################

function(AddSources OutVariableName InPattern)
	set(implPosix "impl_posix")
	set(implWindows "impl_windows")

	file(GLOB_RECURSE fullList "${InPattern}")

	set(commonList "${fullList}")
	list(FILTER commonList EXCLUDE REGEX "\/(${implPosix}|${implWindows})\/")

	set(platformList "${fullList}")
	if(CPPLIB_PLATFORM_UNIX)
		list(FILTER platformList INCLUDE REGEX "\/${implPosix}\/")
	elseif(CPPLIB_PLATFORM_WINDOWS)
		list(FILTER platformList INCLUDE REGEX "\/${implWindows}\/")
	endif()

	set(${OutVariableName} ${commonList} ${platformList} PARENT_SCOPE)
endfunction()

######################################### GENERAL ########################################

if(CPPLIB_COMPILER_CLANG_OR_GCC)
	set(CMAKE_CXX_FLAGS_DEBUG
		"-O0 -g -fno-inline"
		)
	set(CMAKE_CXX_FLAGS_RELEASE
		"-O3"
		)
elseif(CPPLIB_COMPILER_MSVC)
	set(CMAKE_CXX_FLAGS_DEBUG
		"/Od /Zi /Ob0"
		)
	set(CMAKE_CXX_FLAGS_RELEASE
		"/O2"
		)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

if(CPPLIB_PLATFORM_WINDOWS)
	find_library(Sockets NAMES ws2_32 wsock32)
endif()

######################################### LIBRARY ########################################

set(LIBRARY_NAME "${PROJECT_NAME}_static")
AddSources(LIBRARY_SOURCES "include/lib/*.cpp")
set(LIBRARY_INCLUDE_DIRS "include")

if(CPPLIB_COMPILER_CLANG_OR_GCC)
	set(LIBRARY_COMPILE_OPTIONS
		-fno-rtti -fno-exceptions
		-Wall -Wextra -Wpedantic
		-Werror
		-Wdeprecated
		-Wconversion
		-Wno-psabi
		# -Wc++11-compat
		# -Wc++14-compat
		# -Wc++17-compat
		)
elseif(CPPLIB_COMPILER_MSVC)
	set(LIBRARY_COMPILE_OPTIONS
		/GR- /vd0 /D_HAS_EXCEPTIONS=0
		/vmg
		/Wall /external:W0
		/WX
		/D_CRT_SECURE_NO_WARNINGS
		/DNOMINMAX

		/wd4777		# format string '...' requires an argument of type '...', but variadic argument N has type '...'
		/wd4774		# format string expected in argument N is not a string literal
		/wd4566		# character represented by universal-character-name '...' cannot be represented in the current code page
		/wd4738		# storing 32-bit float result in memory, possible loss of performance

		/wd5045		# /Qspectre
		/wd4121		# '...': alignment of a member was sensitive to packing
		/wd4820		# 'N' bytes padding added after base class '...' (EBO)
		/wd4407		# cast between different pointer to member representations, compiler may generate incorrect code
		/wd4868		# compiler may not enforce left-to-right evaluation order in braced initializer list

		/wd4464		# relative include path contains '..'
		/wd4514		# unreferenced inline function has been removed
		/wd4577		# 'noexcept' used with no exception handling mode specified; termination on exception is not guaranteed
		/wd4061		# enumerator '...' in switch of enum '...' is not explicitly handled by a case label
		/wd4371		# layout of class may have changed from a previous version of the compiler due to better packing of member

		/wd4625		# copy constructor was implicitly defined as deleted
		/wd5026		# move constructor was implicitly defined as deleted
		/wd4626		# assignment operator was implicitly defined as deleted
		/wd5027		# move assignment operator was implicitly defined as deleted

		/wd4710		# '...' function not inlined
		/wd4711		# function '...' selected for automatic inline expansion
		/wd4250		# '...' inherits '...' via dominance
		)
endif()

add_compile_options(${LIBRARY_COMPILE_OPTIONS})
include_directories(${LIBRARY_INCLUDE_DIRS})

add_library(${PROJECT_NAME} INTERFACE)
target_compile_options(${PROJECT_NAME}
	INTERFACE ${LIBRARY_COMPILE_OPTIONS})
target_include_directories(${PROJECT_NAME}
	INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_INCLUDE_DIRS}")

add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SOURCES})

########################################## TEST ##########################################

set(TEST_EXECUTABLE "test_${PROJECT_NAME}")
AddSources(TEST_SOURCES "include/test/*.cpp")
list(APPEND TEST_SOURCES "test.cpp")

add_executable(${TEST_EXECUTABLE} ${TEST_SOURCES})
target_link_libraries(${TEST_EXECUTABLE} ${LIBRARY_NAME} Threads::Threads ${Sockets})

add_custom_target(run-test
	COMMAND bash -c 'echo exit | ./${TEST_EXECUTABLE}'
	DEPENDS ${TEST_EXECUTABLE}
	)

######################################## HEXDUMP #########################################

set(HEXDUMP_EXECUTABLE "hexdump_${PROJECT_NAME}")
set(HEXDUMP_SOURCES "hexdump.cpp")

add_executable(${HEXDUMP_EXECUTABLE} ${HEXDUMP_SOURCES})
target_link_libraries(${HEXDUMP_EXECUTABLE} ${LIBRARY_NAME} Threads::Threads)

######################################## GRAPHICS ########################################

if(CPPLIB_PLATFORM_UNIX)
	set(GRAPHICS_PLASMA_EXECUTABLE "graphics_plasma_${PROJECT_NAME}")
	set(GRAPHICS_PLASMA_SOURCES "graphics/plasma.cpp")

	add_executable(${GRAPHICS_PLASMA_EXECUTABLE} ${GRAPHICS_PLASMA_SOURCES})
	target_link_libraries(${GRAPHICS_PLASMA_EXECUTABLE} ${LIBRARY_NAME} Threads::Threads)

	add_custom_target(run-graphics-plasma
		COMMAND ${GRAPHICS_PLASMA_EXECUTABLE}
		DEPENDS ${GRAPHICS_PLASMA_EXECUTABLE}
		)
endif()
