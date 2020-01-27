# Set CMake version
cmake_minimum_required(VERSION 3.14)

# Set project name
set(project_name "demo_circle")
project(${project_name})

# Set the C++ standard to C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Set path to header files directories
include_directories("$ENV{OCCT_INCLUDE}")

# Set path to executable directories
link_directories("$ENV{OCCT_LIB}")

# Add source files to compile to the project
set(SOURCE_FILES main.cpp)
add_executable(${project_name}  ${SOURCE_FILES})

# Add OpenCascade libraries
target_link_libraries(${project_name} -Wl,--no-as-needed
        -lTKernel -lTKMath
        -lTKBRep -lTKG2d -lTKG3d -lTKGeomBase
        -lTKGeomAlgo -lTKTopAlgo -lTKPrim -lTKBO -lTKShHealing -lTKFillet -lTKBool -lTKOffset
        -lTKSTEPBase -lTKXSBase -lTKSTEPAttr -lTKSTEP209 -lTKSTEP -lTKIGES)
