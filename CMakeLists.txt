cmake_minimum_required(VERSION 3.10)
project(a-little-parser CXX)

set(CMAKE_CXX_STANDARD_REQUIRED 14)
set(CMAKE_CXX_STANDARD 17)

if(MSVC)
	set(CMAKE_CXX_FLAGS_DEBUG "/Z7 /Od")
	set(CMAKE_CXX_FLAGS_RELEASE "/Os")
else()
	set(CMAKE_CXX_FLAGS_DEBUG "-Wall -O0 -g -fPIC")
	set(CMAKE_CXX_FLAGS_RELEASE "-Os -fPIC")
endif()

include_directories(src/include)
add_subdirectory(src)
add_subdirectory(examples)