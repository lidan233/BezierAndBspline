cmake_minimum_required(VERSION 3.15)
project(BezierAndBspline)

set(CMAKE_CXX_STANDARD 14)

#find_package(imgui REQUIRED)

include(cmake/LidanBuild.cmake)
include(cmake/LidanBase.cmake)

#ADD_SUBSRC(utildir util)
ADD_SUBSRC(bezierdir bezier)
ADD_SUBSRC(bsplinedir bspline)
ADD_SUBSRC(utildir util)
ADD_SUBSRC(imguidir external/imgui/imgui)


Lidan_ADD_LIB(PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/" INCLUDEDIRS includedir MSVC "TRUE" SOURCEDIR glfwsrc COMPONENTS "glfw3")
Lidan_ADD_SRC(PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/glad/" SOURCEDIR gladsrc)


include_directories(util
                    bezier
                    bspline
                    external/glm/
                    external/eigen/
                    external/imgui/
                    external/imgui/examples)
add_subdirectory(external/imgui/)

add_executable(BezierAndBspline main.cpp
        ${utildir}
        ${bezierdir}
        ${bsplinedir}
        ${glfwsrc}
        ${gladsrc}
        ${imguidir}
        bspline/bplineSurface.cpp bspline/bplineSurface.h ui/ui.h function/FitGeometry.cpp function/FitGeometry.h util/Geometry.cpp util/Geometry.h util/Patch.cpp util/Patch.h)


target_link_libraries(BezierAndBspline imgui glfw3)