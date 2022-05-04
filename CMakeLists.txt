cmake_minimum_required (VERSION 2.6)
project (raytracer)

set(CMAKE_CXX_FLAGS "-std=c++11 -O3")
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )


add_executable(raytracer src/main.cpp
        src/Color.cpp
        src/Triangle.cpp
        src/base.h
        src/KDTree.cpp
        src/Scene.cpp
        src/Image.cpp
        src/Box.cpp
        src/ObjParser.cpp)
target_link_libraries(raytracer LINK_PUBLIC ${OpenCV_LIBS})