# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (MacGame)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_SUPPRESS_REGENERATION ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
endif()

# 定义工程根目录; CMAKE_SOURCE_DIR为内建变量，表示工程根目录的CMakeLists.txt文件路径
SET(ROOT_DIR ${CMAKE_SOURCE_DIR})

# 引入头文件
include_directories(include/
    external/SDL2/
)

# 指定引用的外部库的搜索路径
LINK_DIRECTORIES(${ROOT_DIR}/lib)

file(GLOB PROJECT_HEADERS include/*.h)
file(GLOB PROJECT_SOURCES src/*.cc)
# file(GLOB PROJECT_LIBS lib/*.lib)
file(GLOB PROJECT_LIBS lib/*.dylib)

# add_library(sdl2lib SHARED IMPORTED)
# set_property(TARGET sdl2lib PROPERTY IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/lib/libSDL2.dylib")

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Libs" FILES ${PROJECT_LIBS})

add_definitions(-DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
            
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})

# target_link_libraries(${PROJECT_NAME} ${PROJECT_LIBS} PRIVATE sdl2lib)
target_link_libraries(${PROJECT_NAME} ${PROJECT_LIBS})

set_target_properties(${PROJECT_NAME} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_Debug
            RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_Release)

# 输出
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

if(MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES 
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()

message("[${CMAKE_CURRENT_LIST_DIR}:${CMAKE_CURRENT_LIST_LINE}] CMAKE_CXX_FLAGS ===> ${CMAKE_CXX_FLAGS}")
