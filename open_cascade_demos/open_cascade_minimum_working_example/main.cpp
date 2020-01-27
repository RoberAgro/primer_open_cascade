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
    string open_gui = "freecad --single-instance " + relative_path + file_name + ".step";
    system(open_gui.c_str());


    return 0;


}


