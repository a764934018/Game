cmake_minimum_required(VERSION 3.0.0)
project(Game VERSION 0.1.0)

add_subdirectory(./box2d)

include_directories(./easyx/include ./box2d/include ./box2d/extern)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(PLATFORM x64)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(PLATFORM x86)
endif()

if(NOT DEFINED MSVC_TOOLSET_VERSION)
	message(FATAL_ERROR 检测不到已安装的VS)
endif()
	
if(${MSVC_TOOLSET_VERSION} LESS 90)
    message(FATAL_ERROR "VS版本太老，不支持!")
elseif (${MSVC_TOOLSET_VERSION} LESS 140)
    link_directories(./easyx/lib/VC2008/${PLATFORM})
else()
    link_directories(./easyx/lib/VC2015/${PLATFORM}) 
endif()

link_libraries(box2d)
add_executable(${PROJECT_NAME} main.cpp car.cpp)
