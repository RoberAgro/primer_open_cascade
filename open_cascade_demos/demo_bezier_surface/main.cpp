// ------------------------------------------------------------------------------------------------------------------- //
//
//  Demonstration script showing how to create a Bezier surface in OpenCascade
//  Author: Roberto Agromayor
//
// ------------------------------------------------------------------------------------------------------------------ //


// Include standard C++ libraries
#include <iostream>
#include <sys/stat.h>
#include <cmath>


// Include OpenCascade libraries
#include <gp_Pnt.hxx>
#include <Geom_BezierSurface.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <BRep_Tool.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
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
    // Define the array of control points
    // -------------------------------------------------------------------------------------------------------------- //

    // Declare the array
    Standard_Integer rowLower = 1, rowUpper = 5;
    Standard_Integer colLower = 1, colUpper = 3;
    TColgp_Array2OfPnt P(rowLower, rowUpper, colLower, colUpper);

    // First row
    P(1, 1) = gp_Pnt(0.00, 0.0, 0.0);
    P(2, 1) = gp_Pnt(0.25, 0.0, 0.0);
    P(3, 1) = gp_Pnt(0.50, 0.0, 0.0);
    P(4, 1) = gp_Pnt(0.75, 0.0, 0.0);
    P(5, 1) = gp_Pnt(1.00, 0.0, 0.0);

    // Second row
    P(1, 2) = gp_Pnt(0.00, 0.5, 0.0);
    P(2, 2) = gp_Pnt(0.25, 0.5, 1.0);
    P(3, 2) = gp_Pnt(0.50, 0.5, 1.0);
    P(4, 2) = gp_Pnt(0.75, 0.5, 1.0);
    P(5, 2) = gp_Pnt(1.00, 0.5, 0.0);

    // Third row
    P(1, 3) = gp_Pnt(0.00, 1.0, 0.0);
    P(2, 3) = gp_Pnt(0.25, 1.0, 0.0);
    P(3, 3) = gp_Pnt(0.50, 1.0, 0.0);
    P(4, 3) = gp_Pnt(0.75, 1.0, 0.0);
    P(5, 3) = gp_Pnt(1.00, 1.0, 0.0);


    // -------------------------------------------------------------------------------------------------------------- //
    // Define the geometry and topology of a Bezier surface
    // -------------------------------------------------------------------------------------------------------------- //

    // Define the geometry of a Bezier surface referenced by handle
    Handle(Geom_BezierSurface) BezierGeo = new Geom_BezierSurface(P);

    // Define the topology of the Bezier surface using the BRepBuilderAPI
    TopoDS_Face BezierFace = BRepBuilderAPI_MakeFace(BezierGeo, 0);

    // Get the geometric surface from the topological face and check the bounds in parametric space [Optional]
    Handle(Geom_Surface) BezierGeo_bis = BRep_Tool::Surface (BezierFace);
    double u_lower, u_upper, v_lower, v_upper;
    BezierGeo_bis->Bounds(u_lower, u_upper, v_lower, v_upper);


    // -------------------------------------------------------------------------------------------------------------- //
    // Export the model as a STEP file
    // -------------------------------------------------------------------------------------------------------------- //

    // Create a TopoDS_Shape object to export as .step
    TopoDS_Shape open_cascade_model = BezierFace;

    // Set the destination path and the name of the .step file
    string relative_path = "../output/";
    string file_name = "bezier_surface";

    // Write the .step file
    write_step_file(relative_path, file_name, open_cascade_model);


    // -------------------------------------------------------------------------------------------------------------- //
    // Visualize the geometry in a graphical user interface (for instance the FreeCAD GUI)
    // -------------------------------------------------------------------------------------------------------------- //
    string open_gui = "FreeCAD --single-instance " + relative_path + file_name + ".step";
    system(open_gui.c_str());


    return 0;

}
