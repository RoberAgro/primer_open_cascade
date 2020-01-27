# OpenCascade minimum working example

## 1. Introduction

This document describes how to set up and compile a small, yet complete, C++ project using OpenCascade to check if the installation was successful.

## 2. Creating the CMake project

We will assume that we are building our C++ project using CMake. In order to compile a C++ project using CMake from an IDE (such as CLion or Visual Studio) we just need to write a small `CMakeLists.txt` file. In our case, this file should look something like this:

```cpp
# Set CMake version
cmake_minimum_required(VERSION 3.14)

# Set project name
set(project_name "open_cascade_minimum_working_example")
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

```

Where the environmental variables `OCCT_INCLUDE` and `OCCT_LIB` point to the directories containing the OpenCascade headers and compiled libraries, respectively.


## Defining the environmental variables
You can define the environmental variables used in the `CMakeLists.txt` file adding (something similar) to these lines to your `.bashrc` file:

	# Path to the OpenCascade installation directory
	export OCCT_INCLUDE="/home/robertoa/my_software/open_cascade-7.4.0/open_cascade_installation/include/opencascade"
	export OCCT_LIB="/home/robertoa/my_software/open_cascade-7.4.0/open_cascade_installation/lib"



## Writing the C++ program

Now that we have prepared the `CMakeLists.txt` file of the project we can write our C++ code.
The code snippet below is a minimum working example showing how to create a box using OpenCascade.
You should be able to compile this example straight ahead if the installation was successful.
Check out the [detailed explanation of the minimum working example](./open_cascade_minimum_working_example_explanation.md) if you want to learn more about how OpenCascade works. 



```cpp
// ------------------------------------------------------------------------------------------------------------------- //
//
//  Minimum working example showing the use of OpenCascade
//  Author: Roberto Agromayor
//
// ------------------------------------------------------------------------------------------------------------------ //


// Include standard C++ libraries
#include <iostream>
#include <sys/stat.h>
#include <cmath>


// Include OpenCascade libraries
#include <gp_Pnt.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <STEPControl_Writer.hxx>


// Define namespaces
using namespace std;


// ------------------------------------------------------------------------------------------------------------------ //
// Step file exporter
// ------------------------------------------------------------------------------------------------------------------ //
IFSelect_ReturnStatus
write_step_file(const string &relative_path, const string &model_name, const TopoDS_Shape &model_object) {

    // Create the .step writer object
    STEPControl_Writer step_writer;

    // Set the type of .step representation
    STEPControl_StepModelType step_mode = STEPControl_StepModelType::STEPControl_AsIs;

    // Create the output directory if it does not exist
    mkdir(relative_path.c_str(), 0777);     // 0007 is used to give the user permissions to read+write+execute

    // Get the full path to the step file as a C-string
    string temp = (relative_path + model_name + ".step");
    Standard_CString file_name = temp.c_str();

    // Write the .step file
    step_writer.Transfer(model_object, step_mode);
    IFSelect_ReturnStatus status = step_writer.Write(file_name);
    return status;

}


// ------------------------------------------------------------------------------------------------------------------ //
// Main body
// ------------------------------------------------------------------------------------------------------------------ //
int main() {

    
    // -------------------------------------------------------------------------------------------------------------- //
    // Define the geometry and topology of a prism
    // -------------------------------------------------------------------------------------------------------------- //

    // Define points that define the geometry of the base-square
    gp_Pnt myPoint1(0., 0., 0.);   // Bottom left
    gp_Pnt myPoint2(1., 0., 0.);   // Bottom right
    gp_Pnt myPoint3(1., 1., 0.);   // Top right
    gp_Pnt myPoint4(0., 1., 0.);   // Bottom left

    // Create the edges using gp_Pnt objects and BRepBuilderAPI_MakeEdge()
    TopoDS_Edge myEdge1 = BRepBuilderAPI_MakeEdge(myPoint1, myPoint2);
    TopoDS_Edge myEdge2 = BRepBuilderAPI_MakeEdge(myPoint2, myPoint3);
    TopoDS_Edge myEdge3 = BRepBuilderAPI_MakeEdge(myPoint3, myPoint4);
    TopoDS_Edge myEdge4 = BRepBuilderAPI_MakeEdge(myPoint4, myPoint1);

    // Create a closed wire giving the edges to the constructor
    TopoDS_Wire myWire = BRepBuilderAPI_MakeWire(myEdge1, myEdge2, myEdge3, myEdge4);

    // Make a face from the plane wire
    TopoDS_Face myFace = BRepBuilderAPI_MakeFace(myWire);

    // Make a prism sweeping the base-square
    gp_Vec sweep_direction(0.00, 0.00, 1.00);
    TopoDS_Shape myPrism = BRepPrimAPI_MakePrism(myFace, sweep_direction);


    // -------------------------------------------------------------------------------------------------------------- //
    // Export the model as a STEP file
    // -------------------------------------------------------------------------------------------------------------- //

    // Set the destination path and the name of the .step file
    string relative_path = "../output/";
    string file_name = "minimum_working_example";

    // Write the .step file
    write_step_file(relative_path, file_name, myPrism);


    // -------------------------------------------------------------------------------------------------------------- //
    // Visualize the geometry in a graphical user interface (for instance the FreeCAD GUI)
    // -------------------------------------------------------------------------------------------------------------- //
    string open_gui = "FreeCAD --single-instance " + relative_path + file_name + ".step";
    system(open_gui.c_str());


    return 0;


}
```


